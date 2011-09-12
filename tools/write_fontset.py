#!/usr/bin/env python

"""
Writes a binary file which contains a binary representation 
of the chip8 fontset.
"""

import binascii

file = open("fontset.bin", "wb")

charset = {}

charset[0x0] = "F0909090F0" 
charset[0x1] = "2060202070"
charset[0x2] = "F010F080F0"
charset[0x3] = "F010F010F0"
charset[0x4] = "9090F01010"
charset[0x5] = "F090F010F0"
charset[0x6] = "F080F090F0"
charset[0x7] = "F010204040"
charset[0x8] = "F090F090F0"
charset[0x9] = "F090F010F0"
charset[0xA] = "F090F09090"
charset[0xB] = "E090E090E0"
charset[0xC] = "F0808080F0"
charset[0xD] = "E0909090E0"
charset[0xE] = "F080F080F0"
charset[0xF] = "F080F08080"

for k, v in charset.items():
    print "%s=%s" % (k, v)
    b = binascii.a2b_hex(v)
    file.write(b)

file.close()
