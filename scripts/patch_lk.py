#!/usr/bin/env python3

import struct
import sys

base = 0x4C400000
hdr_sz = 0x200

pivot = 0x4c4262a8  # platform_init() caller
heap_start = 0x4C5B8710  # merely informative

# TODO: Relocate the payload to a better location
#       (e.g. after the LK code, before the heap)
#       since a big enough payload will overwrite
#       the heap and cause instability.
inject_addr = 0x4C4F6400 - base + hdr_sz


def encode_bl(src, dst):
    off = dst - (src + 4)
    hi, lo = (off >> 12) & 0x7FF, (off >> 1) & 0x7FF
    return struct.pack('<HH', 0xF000 | hi, 0xF800 | lo)


def main():
    output = sys.argv[3] if len(sys.argv) == 4 else sys.argv[1] + '-patched'

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())
        size = len(data)

    with open(sys.argv[2], 'rb') as f:
        payload = f.read()

    magic, code_sz = struct.unpack('<II', data[:8])
    name = data[8:40].decode('utf-8').rstrip('\0')
    print('base: 0x%08x, size: %d, name: %s' % (base, code_sz, name))
    assert name.lower() == 'lk', 'Invalid LK'

    print('payload injection point at 0x%08x' % ((base + inject_addr) - hdr_sz))
    data[inject_addr : inject_addr + len(payload)] = payload

    shellcode = encode_bl(pivot, (base + inject_addr) - hdr_sz)
    print(
        '0x%08x (pivot) bl 0x%08x (payload)'
        % (pivot, (base + inject_addr) - hdr_sz)
    )
    offset = (pivot - base) + hdr_sz
    data[offset : offset + len(shellcode)] = shellcode

    print('Writing %d bytes to %s...' % (len(data), output))
    with open(output, 'wb') as f:
        f.write(data)


if __name__ == '__main__':
    main()
