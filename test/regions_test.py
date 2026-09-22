"""Exercise the actual C resolver against ELF-loaded images, not live RAM captures."""
from pathlib import Path
import argparse
import ctypes as C
import hashlib
import json
import struct
import subprocess
import tempfile

LIB = Path(__file__).resolve().parents[1]
RAM_SIZE = 0x02000000
U32 = C.c_uint32


class Memory(C.Structure):
    _fields_ = [('base', C.c_void_p), ('size', U32)]


class Module(C.Structure):
    _fields_ = [(name, U32) for name in ('header', 'table', 'source', 'count', 'sourceHash')]


class Context(C.Structure):
    _fields_ = [('memory', Memory), ('boot', Module), ('ready', C.c_int)]


class Build(C.Structure):
    _fields_ = [('region', C.c_int), ('name', C.c_char_p), ('serial', C.c_char_p),
                ('version', C.c_char_p), ('entryPoint', U32), ('symbolTable', U32), ('symbolCount', U32)]


def load_elf(data):
    assert data[:6] == b'\x7fELF\x01\x01'
    image = bytearray(RAM_SIZE)
    phoff = struct.unpack_from('<I', data, 28)[0]
    size, count = struct.unpack_from('<HH', data, 42)
    for i in range(count):
        kind, off, va, pa, fs, ms, flags, align = struct.unpack_from('<8I', data, phoff+i*size)
        if kind != 1:
            continue
        assert fs <= ms and va+ms <= len(image) and off+fs <= len(data)
        image[va:va+fs] = data[off:off+fs]
    return image


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--cc', default='cc')
    ap.add_argument('--sm-root', type=Path, default=LIB.parent/'test/sm')
    args = ap.parse_args()
    profiles = json.loads((LIB/'docs/region-profiles.json').read_text())
    report = {'method': 'Host C resolver on PT_LOAD-mapped real boot ELFs; not runtime execution.', 'regions': []}
    with tempfile.TemporaryDirectory() as temp:
        for target in ('auto', 'ntscu', 'pal', 'ntscj'):
            output = Path(temp)/('resolver-'+target+'.dll')
            subprocess.run([args.cc, '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC',
                            '-DRAC5_'+target.upper(), '-I'+str(LIB/'include'),
                            *map(str, sorted((LIB/'src').glob('*.c'))), '-o', str(output)], check=True)
            dll = C.CDLL(str(output))
            dll.rac5Init.argtypes = [C.POINTER(Context), C.c_void_p, U32]
            dll.rac5Init.restype = C.c_int
            dll.rac5Resolve.argtypes = [C.POINTER(Context), C.c_char_p]
            dll.rac5Resolve.restype = U32
            dll.rac5GetBuild.argtypes = [C.POINTER(Context)]
            dll.rac5GetBuild.restype = C.POINTER(Build)
            dll.rac5BuildRegion.restype = C.c_char_p
            dll.rac5LevelSource.argtypes = [C.c_int]
            dll.rac5LevelSource.restype = C.c_char_p
            dll.rac5GetCurrentLevel.argtypes = [C.POINTER(Context)]
            dll.rac5GetCurrentLevel.restype = C.c_int
            dll.rac5GetAddress.argtypes = [C.POINTER(Context), C.c_void_p]
            dll.rac5GetAddress.restype = U32
            dll.rac5GetAddressForLevel.argtypes = [C.c_void_p, C.c_int]
            dll.rac5GetAddressForLevel.restype = U32
            dll.rac5SetAddressForLevel.argtypes = [C.c_void_p, C.c_int, U32]
            dll.rac5SetAddressForLevel.restype = C.c_int
            assert dll.rac5BuildRegion().decode() == target
            assert not dll.rac5GetBuild(None)
            for index, profile in enumerate(profiles, 1):
                region = profile['region']
                root = args.sm_root if region == 'ntscu' else args.sm_root/region
                data = (root/'extracted/disc'/profile['serial']).read_bytes()
                assert hashlib.sha256(data).hexdigest() == profile['elf_sha256']
                image = load_elf(data)
                ram = (C.c_ubyte*RAM_SIZE).from_buffer(image)
                ctx = Context()
                assert dll.rac5Init(C.byref(ctx), ram, RAM_SIZE)
                found = dll.rac5GetBuild(C.byref(ctx))
                assert found and found.contents.region == index
                assert found.contents.name.decode() == region
                assert found.contents.serial.decode() == profile['serial']
                assert found.contents.version == b'1.00'
                assert found.contents.entryPoint == profile['entry_point']
                assert found.contents.symbolTable == profile['symbol_table']
                assert found.contents.symbolCount == profile['symbol_count']
                symbols = json.loads((root/'symbols/symbols.json').read_text())['symbols']
                by_name = {s['name']: s for s in symbols}
                for symbol in symbols:
                    assert dll.rac5Resolve(C.byref(ctx), symbol['name'].encode()) == symbol['address'], symbol
                assert dll.rac5Resolve(C.byref(ctx), b'not_a_real_game_symbol') == 0
                # Changing an anchor invalidates known-build identification; do not cache a guess.
                address = ctx.boot.table+by_name['memset']['index']*12+4
                saved = image[address:address+4]
                image[address:address+4] = b'\0'*4
                assert not dll.rac5GetBuild(C.byref(ctx))
                image[address:address+4] = saved
                assert dll.rac5GetBuild(C.byref(ctx))
                # Decode the exact getter and exercise every actual table index.
                getter = by_name['MCPTRANS_GetLevel__Fv']['address']
                hi, jr, low, nop = struct.unpack_from('<4I', image, getter)
                assert hi & 0xffff0000 == 0x3c030000 and jr == 0x03e00008
                assert low & 0xffff0000 == 0x8c620000 and nop == 0
                level_global = ((hi & 0xffff) << 16) + struct.unpack('<h', struct.pack('<H', low & 0xffff))[0]
                original_level = image[level_global:level_global+4]
                table = (U32*25)(*[0x100000+i*16 for i in range(25)])
                for level in range(25):
                    struct.pack_into('<I', image, level_global, level)
                    assert dll.rac5GetCurrentLevel(C.byref(ctx)) == level
                    assert dll.rac5GetAddress(C.byref(ctx), table) == table[level]
                    assert dll.rac5SetAddressForLevel(table, level, 0x200000+level)
                    assert dll.rac5GetAddressForLevel(table, level) == 0x200000+level
                assert dll.rac5GetAddressForLevel(table, -1) == table[0]
                for invalid in (25, 26, 27, 28, 29, 0xffffffff):
                    struct.pack_into('<I', image, level_global, invalid)
                    assert dll.rac5GetCurrentLevel(C.byref(ctx)) == -2
                    assert dll.rac5GetAddress(C.byref(ctx), table) == 0
                assert not dll.rac5SetAddressForLevel(table, 25, 1)
                image[level_global:level_global+4] = original_level
                image[getter] ^= 1
                assert dll.rac5GetCurrentLevel(C.byref(ctx)) == -2
                image[getter] ^= 1
                execs = by_name['levelExecs']['address']
                paths = []
                for level in range(25):
                    pointer = struct.unpack_from('<I', image, execs+level*4)[0]
                    paths.append(bytes(image[pointer:image.index(0, pointer)]).decode())
                assert struct.unpack_from('<I', image, execs+25*4)[0] == 0
                assert paths[0] == 'cdrom0:/LVL/FRONTEND.REL'
                for level in list(range(1,11))+list(range(15,25)):
                    assert paths[level] == f'cdrom0:/LVL/LEVEL_{level:02d}.REL'
                assert paths[11] == paths[12] == paths[10]
                if target == 'auto':
                    missing = {}
                    imports = 0
                    modules = list((root/'extracted/disc/LVL').glob('*.REL'))
                    assert len(modules) == 21
                    for path in modules:
                        rel = path.read_bytes()
                        tab, count, source = struct.unpack_from('<III', rel, 12)
                        level = -1 if path.stem == 'FRONTEND' else int(path.stem.split('_')[1])
                        expected_source = rel[source:rel.index(b'\0', source)]
                        assert dll.rac5LevelSource(level) == expected_source
                        for i in range(count):
                            np, value, a, b, kind, processed = struct.unpack_from('<IIBBBB', rel, tab+i*12)
                            if not np or kind != 1:
                                continue
                            name = rel[np:rel.index(b'\0', np)].decode()
                            if name not in by_name:
                                missing.setdefault(name, []).append(path.name)
                            else:
                                assert dll.rac5Resolve(C.byref(ctx), name.encode()) == by_name[name]['address']
                                imports += 1
                    assert set(missing) == {'PAUSEMODE_GetCurrentPauseScreen__Fv'}, missing
                    report['regions'].append({**profile, 'all_named_symbols_resolved': len(symbols),
                                              'level_sources_checked': len(modules),
                                              'boot_resolvable_import_records': imports,
                                              'imports_absent_from_boot': missing, 'current_level_global': hex(level_global),
                                              'runtime_level_paths': paths, 'variable_address_slots_checked': 25})
                print(f'{target}: {region} ELF build identified; {len(symbols)} symbols verified', flush=True)
            # FreeLibrary before temporary-directory cleanup on Windows.
            if hasattr(C, 'WinDLL'):
                import _ctypes
                _ctypes.FreeLibrary(dll._handle)
    (LIB/'docs/regional-verification.json').write_text(json.dumps(report, indent=2)+'\n')


if __name__ == '__main__':
    main()
