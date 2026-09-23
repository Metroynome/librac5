import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile

LIB = Path(__file__).resolve().parents[1]
ROOT = LIB.parent
sys.path.insert(0, str(LIB/'tools'))
from update_game_exports import EXPORTS


def sections(path):
    data = Path(path).read_bytes()
    off = struct.unpack_from('<I', data, 32)[0]
    size, count, names = struct.unpack_from('<HHH', data, 46)
    rows = [struct.unpack_from('<10I', data, off+i*size) for i in range(count)]
    strings = data[rows[names][4]:rows[names][4]+rows[names][5]]
    return {strings[r[0]:strings.index(0, r[0])].decode(): data[r[4]:r[4]+r[5]] for r in rows if r[1] != 8}


def load_elf(path):
    data = path.read_bytes()
    image = bytearray(0x2000000)
    off = struct.unpack_from('<I', data, 28)[0]
    size, count = struct.unpack_from('<HH', data, 42)
    for i in range(count):
        kind, pos, va, _, fs, ms, _, _ = struct.unpack_from('<8I', data, off+i*size)
        if kind == 1:
            assert fs <= ms and va+ms <= len(image)
            image[va:va+fs] = data[pos:pos+fs]
    return image


def word(data, address):
    return struct.unpack_from('<I', data, address)[0]


def put(data, address, value):
    struct.pack_into('<I', data, address, value & 0xffffffff)


def execute(code, ram, table):
    # Execute the compiled leaf resolver, including MIPS branch delay slots.
    r = [0]*32
    r[4], r[31] = table, 0xffffffff
    pc, pending, lo = 0, None, 0
    for _ in range(300):
        if pc == 0xffffffff:
            return r[2]
        w = word(code, pc)
        op, rs, rt, rd, shift, fn = w>>26, w>>21&31, w>>16&31, w>>11&31, w>>6&31, w&63
        imm = w & 65535
        signed = imm-65536 if imm & 32768 else imm
        next_pc, pending = (pending if pending is not None else pc+4), None
        if op == 0:
            if fn == 0: r[rd] = r[rt] << shift
            elif fn == 8: pending = r[rs]
            elif fn == 0x12: r[rd] = lo
            elif fn == 0x18: lo = r[rs]*r[rt] & 0xffffffff
            elif fn == 0x21: r[rd] = r[rs]+r[rt]
            elif fn == 0x23: r[rd] = r[rs]-r[rt]
            elif fn == 0x25: r[rd] = r[rs] | r[rt]
            elif fn == 0x2b: r[rd] = int(r[rs] < r[rt])
            else: raise AssertionError(hex(w))
        elif op in (4, 5):
            if (r[rs] == r[rt]) == (op == 4): pending = pc+4+signed*4
        elif op == 9: r[rt] = r[rs]+signed
        elif op == 11: r[rt] = int(r[rs] < (signed & 0xffffffff))
        elif op == 12: r[rt] = r[rs] & imm
        elif op == 13: r[rt] = r[rs] | imm
        elif op == 15: r[rt] = imm << 16
        elif op == 35: r[rt] = word(ram, (r[rs]+signed) & 0xffffffff)
        else: raise AssertionError(hex(w))
        r = [x & 0xffffffff for x in r]
        r[0], pc = 0, next_pc
    raise AssertionError('Resolver did not return')


def main():
    prefix = os.environ.get('EE_PREFIX', 'mips64r5900el-ps2-elf-')
    aliases = dict(EXPORTS, gameGetCurrentLevel='MCPTRANS_GetLevel__Fv')
    for region, serial in [('ntscu','SCUS_976.15'), ('pal','SCES_550.19'), ('ntscj','SCPS_151.20')]:
        root = ROOT/'test/sm'/('' if region == 'ntscu' else region)
        image = load_elf(root/'extracted/disc'/serial)
        symbols = {r['name']:r['address'] for r in json.loads((root/'symbols/symbols.json').read_text())['symbols']}
        obj = LIB/'obj'/region
        funcs = sections(obj/'functions.o')
        defines = subprocess.check_output([prefix+'gcc','-E','-dM','-DRAC5_'+region.upper(),str(LIB/'src/functions.S')], text=True)
        globals = {k:int(v, 0) for k,v in re.findall(r'#define (GAME_\w+) (0x[0-9a-f]+|\d+)\b',defines)}
        for define, name in [('GAME_MAP_ID','MCPTRANS_GetLevel__Fv'), ('GAME_MODULE_ID','MCPI_GetGameId__Fv')]:
            a = symbols[name]
            lo = word(image,a+8) & 65535
            address = ((word(image,a)&65535)<<16)+(lo-65536 if lo&32768 else lo)
            assert globals[define] == address
        assert globals['GAME_MODULES'] == symbols['modules']
        assert globals['GAME_LEVEL_TO_LAUNCH'] == symbols['g_LevelToLaunch']
        exports = 0
        for name, code in funcs.items():
            if name.startswith('.text.') and len(code) == 8:
                name = name[6:]
                assert word(code,0)>>26 == 2 and word(code,4) == 0
                assert (word(code,0)&0x3ffffff)<<2 == symbols[aliases.get(name,name)], name
                exports += 1
        tables = {}
        for filename, names in [('message',['vaMsgStr']), ('moby',['vaMobyCreate','vaMobyDestroy'])]:
            data = sections(obj/(filename+'.o'))
            for name in names:
                tables[name] = struct.unpack('<25I',data['.data.'+name])
        code = funcs['.text.GetAddress']
        base, ptr, record = 0x400000, 0x1000, globals['GAME_MODULES']+0x418
        put(image, globals['GAME_MODULE_ID'],1)
        put(image,record+4,base);put(image,record+8,1);put(image,base,0x32524e53)
        for table in tables.values():
            struct.pack_into('<25I',image,ptr,*table)
            for level in range(25):
                put(image,globals['GAME_MAP_ID'],level);put(image,globals['GAME_LEVEL_TO_LAUNCH'],level)
                assert execute(code,image,ptr) == (base+table[level] if table[level] else 0)
        # Invalid IDs, loading transitions, unloaded modules, bad pointers and overflow.
        struct.pack_into('<25I',image,ptr,*tables['vaMsgStr'])
        put(image,globals['GAME_MAP_ID'],1);put(image,globals['GAME_LEVEL_TO_LAUNCH'],1)
        for address, values in [(globals['GAME_MAP_ID'],[-1,25]), (globals['GAME_MODULE_ID'],[-1,8]), (globals['GAME_LEVEL_TO_LAUNCH'],[2]), (record+8,[0]), (record+4,[0,0x400001,0x2000000]), (base,[0]), (ptr+4,[0xffffffff])]:
            old = word(image,address)
            for value in values:
                put(image,address,value)
                assert execute(code,image,ptr) == 0, (region,hex(address),value)
            put(image,address,old)
        assert execute(code,image,0) == 0
        if region == 'ntscu':
            for dump in ['D:/01_pokitaru_eeMemory.bin','D:/16c_island_escape_eeMemory.bin','D:/17c_danger_valley_eeMemory.bin',str(ROOT/'test/sm/symbols/full_audit/mp_eeMemory.bin')]:
                ram = bytearray(Path(dump).read_bytes())
                level = word(ram,globals['GAME_MAP_ID'])
                module = word(ram,globals['GAME_MODULES']+word(ram,globals['GAME_MODULE_ID'])*0x418+4)
                for table in tables.values():
                    struct.pack_into('<25I',ram,ptr,*table)
                    assert execute(code,ram,ptr) == module+table[level]
        with tempfile.TemporaryDirectory() as tmp:
            out = str(Path(tmp)/'all.o')
            libgcc = subprocess.check_output([prefix+'gcc','-print-libgcc-file-name'],text=True).strip()
            subprocess.run([prefix+'ld','-r','--whole-archive',str(LIB/'lib'/('librac5'+region+'.a')),'--no-whole-archive',libgcc,'-o',out],check=True)
            assert not subprocess.check_output([prefix+'nm','-u',out],text=True).strip()
        print(region, exports, 'fixed exports; 75 table selections; invalid-state and whole-library link checks passed')
    hashes = json.loads((ROOT/'test/sm/librac5_preserved_hashes.json').read_text())
    for file, expected in hashes.items():
        assert hashlib.sha256((ROOT/file).read_bytes()).hexdigest() == expected, file
    assert not list(LIB.rglob('*.inc'))
    print('Four RAM dumps passed; all five user files are byte-for-byte unchanged.')


if __name__ == '__main__':
    main()
