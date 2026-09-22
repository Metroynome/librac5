"""Regenerate reference profiles from the locally extracted regional boot ELFs."""
from pathlib import Path
import argparse
import hashlib
import json
import struct

LIB = Path(__file__).resolve().parents[1]
ANCHORS = ('memset', 'printf', 'MCP_LoadLevelModule__Fi', 'MCPI_SendNewLevel__Fi')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--sm-root', type=Path, default=LIB.parent / 'test/sm')
    args = ap.parse_args()
    profiles = []
    lines = ['/* Generated from verified regional ELF symbols; do not hand-edit. */',
             'static const KnownBuild knownBuilds[] = {']
    for region, serial in [('ntscu', 'SCUS_976.15'), ('pal', 'SCES_550.19'), ('ntscj', 'SCPS_151.20')]:
        root = args.sm_root if region == 'ntscu' else args.sm_root / region
        symbols = json.loads((root / 'symbols/symbols.json').read_text())
        data = (root / 'extracted/disc' / serial).read_bytes()
        assert hashlib.sha256(data).hexdigest() == symbols['sha256']
        assert data[:6] == b'\x7fELF\x01\x01'
        offset = struct.unpack_from('<I', data, 32)[0]
        size, count, strings = struct.unpack_from('<HHH', data, 46)
        sections = [struct.unpack_from('<10I', data, offset + i*size) for i in range(count)]
        s = sections[strings]; names = data[s[4]:s[4]+s[5]]
        sndata = next(s for s in sections if names[s[0]:].split(b'\0')[0] == b'.sndata')
        assert data[sndata[4]:sndata[4]+4] == b'SNR2'
        assert struct.unpack_from('<I', data, sndata[4]+16)[0] == symbols['record_count']
        by_name = {r['name']: r['address'] for r in symbols['symbols']}
        entry = struct.unpack_from('<I', data, 24)[0]
        profile = dict(region=region, serial=serial, version='1.00', elf_sha256=symbols['sha256'],
                       entry_point=entry, symbol_table=sndata[3], symbol_count=symbols['record_count'],
                       named_symbols=len(symbols['symbols']), anchors={n: by_name[n] for n in ANCHORS})
        profiles.append(profile)
        lines.append('    {{RAC5_REGION_%s, "%s", "%s", "1.00", 0x%08xu, 0x%08xu, %du}, %s},' %
                     (region.upper(), region, serial, entry, sndata[3], symbols['record_count'],
                      ', '.join('0x%08xu' % by_name[n] for n in ANCHORS)))
        (LIB / 'docs' / ('symbols-'+region+'.tsv')).write_bytes((root/'symbols/symbols.tsv').read_bytes())
    lines.append('};')
    (LIB/'src/region_profiles.inc').write_text('\n'.join(lines)+'\n', newline='\n')
    (LIB/'docs/region-profiles.json').write_text(json.dumps(profiles, indent=2)+'\n', newline='\n')
    print('Generated verified reference profiles for NTSC-U, PAL and NTSC-J')


if __name__ == '__main__':
    main()
