#!/usr/bin/env python3

# SPDX-FileCopyrightText: Copyright 2024 polyproxy
# SPDX-License-Identifier: MIT

import argparse

REGISTERS = {
    # Vertex info
    0x00: 'PRIM',
    0x01: 'RGBAQ',
    0x02: 'ST',
    0x03: 'UV',
    0x04: 'XYZF2',
    0x05: 'XYZ2',
    0x0a: 'FOG',
    0x0c: 'XYZF3',
    0x0d: 'XYZ3',
    0x18: 'XYOFFSET_1',
    0x19: 'XYOFFSET_2',
    0x1a: 'PRMODECONT',

    # Drawing attributes
    0x1b: 'PRMODE',
    0x06: 'TEX0_1',
    0x07: 'TEX0_2',
    0x14: 'TEX1_1',
    0x15: 'TEX1_2',
    0x16: 'TEX2_1',
    0x17: 'TEX2_2',
    0x1c: 'TEXCLUT',
    0x22: 'SCANMSK',
    0x34: 'MIPTBP1_1',
    0x35: 'MIPTBP1_2',
    0x36: 'MIPTBP2_1',
    0x37: 'MIPTBP2_2',
    0x08: 'CLAMP_1',
    0x09: 'CLAMP_2',
    0x3b: 'TEXA',
    0x3d: 'FOGCOL',
    0x3f: 'TEXFLUSH',

    # Pixel operations
    0x40: 'SCISSOR_1',
    0x41: 'SCISSOR_2',
    0x42: 'ALPHA_1',
    0x43: 'ALPHA_2',
    0x44: 'DIMX',
    0x45: 'DTHE',
    0x46: 'COLCLAMP',
    0x47: 'TEST_1',
    0x48: 'TEST_2',
    0x49: 'PABE',
    0x4a: 'FBA_1',
    0x4b: 'FBA_2',

    # Buffer
    0x4c: 'FRAME_1',
    0x4d: 'FRAME_2',
    0x4e: 'ZBUF_1',
    0x4f: 'ZBUF_2',

    # Inter-buffer transfer
    0x50: 'BITBLTBUF',
    0x51: 'TRXPOS',
    0x52: 'TRXREG',
    0x53: 'TRXDIR',
    0x54: 'HWREG',

    # Other
    0x60: 'SIGNAL',
    0x61: 'FINISH',
    0x62: 'LABEL',
    0x7f: 'NOP',
}

REVERSE_REGISTERS = {v: k for k, v in REGISTERS.items()}

def format_macro(register_name, *args):
    args_str = ', '.join(map(str, args))
    macro = f"SCE_GS_SET_{register_name.upper()}({args_str})"
    return macro

def parse_hex(hex_data):
    try:
        return int(hex_data, 16)
    except ValueError:
        raise ValueError("Invalid register data")

def disassemble_null(data, name):
    return f"Unimplemented register {name}"

def disassemble_prim(data, name):
    PRIM = (data >> 0 ) & 0b111 # bits 0-2
    IIP  = (data >> 3 ) & 0b1   # bit 3
    TME  = (data >> 4 ) & 0b1   # bit 4
    FGE  = (data >> 5 ) & 0b1   # bit 5
    ABE  = (data >> 6 ) & 0b1   # bit 6
    AA1  = (data >> 7 ) & 0b1   # bit 7
    FST  = (data >> 8 ) & 0b1   # bit 8
    CTXT = (data >> 9 ) & 0b1   # bit 9
    FIX  = (data >> 10) & 0b1   # bit 10

    return format_macro(name, PRIM, IIP, TME, FGE, ABE, AA1, FST, CTXT, FIX)

def disassemble_tex1(data, name):
    LCM  = (data >> 0 ) & 0b1   # bit 0
    MXL  = (data >> 2 ) & 0b111 # bits 2-4
    MMAG = (data >> 5 ) & 0b1   # bit 5
    MMIN = (data >> 6 ) & 0b111 # bits 6-8
    MTBA = (data >> 9 ) & 0b1   # bit 9
    L    = (data >> 19) & 0b11  # bits 19-20
    K    = (data >> 32) & 12    # bits 32-43

    return format_macro(name, LCM, MXL, MMAG, MMIN, MTBA, L, K)

def disassemble_alpha(data, name):
    A   = (data >> 0 ) & 0b11 # bits 0-1
    B   = (data >> 2 ) & 0b11 # bits 2-3
    C   = (data >> 4 ) & 0b11 # bits 4-5
    D   = (data >> 6 ) & 0b11 # bits 6-7
    FIX = (data >> 32) & 0xff # bits 32-39

    return format_macro(name, A, B, C, D, FIX)
    
DISASSEMBLY_FUNCTIONS = {
    # Vertex info
    'PRIM':       lambda data: disassemble_prim(data, 'PRIM'),
    'RGBAQ':      lambda data: disassemble_null(data, 'RGBAQ'),
    'ST':         lambda data: disassemble_null(data, 'ST'),
    'UV':         lambda data: disassemble_null(data, 'UV'),
    'XYZF2':      lambda data: disassemble_null(data, 'XYZF2'),
    'XYZ2':       lambda data: disassemble_null(data, 'XYZ2'),
    'FOG':        lambda data: disassemble_null(data, 'FOG'),
    'XYZF3':      lambda data: disassemble_null(data, 'XYZF3'),
    'XYZ3':       lambda data: disassemble_null(data, 'XYZ3'),
    'XYOFFSET_1': lambda data: disassemble_null(data, 'XYOFFSET_1'),
    'XYOFFSET_2': lambda data: disassemble_null(data, 'XYOFFSET_2'),
    'PRMODECONT': lambda data: disassemble_null(data, 'PRMODECONT'),

    # Drawing attributes
    'PRMODE':     lambda data: disassemble_null(data, 'PRMODE'),
    'TEX0_1':     lambda data: disassemble_null(data, 'TEX0_1'),
    'TEX0_2':     lambda data: disassemble_null(data, 'TEX0_2'),
    'TEX1_1':     lambda data: disassemble_tex1(data, 'TEX1_1'),
    'TEX1_2':     lambda data: disassemble_tex1(data, 'TEX1_2'),
    'TEX2_1':     lambda data: disassemble_null(data, 'TEX2_1'),
    'TEX2_2':     lambda data: disassemble_null(data, 'TEX2_2'),
    'TEXCLUT':    lambda data: disassemble_null(data, 'TEXCLUT'),
    'SCANMSK':    lambda data: disassemble_null(data, 'SCANMSK'),
    'MIPTBP1_1':  lambda data: disassemble_null(data, 'MIPTBP1_1'),
    'MIPTBP1_2':  lambda data: disassemble_null(data, 'MIPTBP1_2'),
    'MIPTBP2_1':  lambda data: disassemble_null(data, 'MIPTBP2_1'),
    'MIPTBP2_2':  lambda data: disassemble_null(data, 'MIPTBP2_2'),
    'CLAMP_1':    lambda data: disassemble_null(data, 'CLAMP_1'),
    'CLAMP_2':    lambda data: disassemble_null(data, 'CLAMP_2'),
    'TEXA':       lambda data: disassemble_null(data, 'TEXA'),
    'FOGCOL':     lambda data: disassemble_null(data, 'FOGCOL'),
    'TEXFLUSH':   lambda data: disassemble_null(data, 'TEXFLUSH'),

    # Pixel operations
    'SCISSOR_1':  lambda data: disassemble_null(data, 'SCISSOR_1'),
    'SCISSOR_2':  lambda data: disassemble_null(data, 'SCISSOR_2'),
    'ALPHA_1':    lambda data: disassemble_alpha(data, 'ALPHA_1'),
    'ALPHA_2':    lambda data: disassemble_alpha(data, 'ALPHA_2'),
    'DIMX':       lambda data: disassemble_null(data, 'DIMX'),
    'DTHE':       lambda data: disassemble_null(data, 'DTHE'),
    'COLCLAMP':   lambda data: disassemble_null(data, 'COLCLAMP'),
    'TEST_1':     lambda data: disassemble_null(data, 'TEST_1'),
    'TEST_2':     lambda data: disassemble_null(data, 'TEST_2'),
    'PABE':       lambda data: disassemble_null(data, 'PABE'),
    'FBA_1':      lambda data: disassemble_null(data, 'FBA_1'),
    'FBA_2':      lambda data: disassemble_null(data, 'FBA_2'),

    # Buffer
    'FRAME_1':    lambda data: disassemble_null(data, 'FRAME_1'),
    'FRAME_2':    lambda data: disassemble_null(data, 'FRAME_2'),
    'ZBUF_1':     lambda data: disassemble_null(data, 'ZBUF_1'),
    'ZBUF_2':     lambda data: disassemble_null(data, 'ZBUF_2'),

    # Inter-buffer transfer
    'BITBLTBUF':  lambda data: disassemble_null(data, 'BITBLTBUF'),
    'TRXPOS':     lambda data: disassemble_null(data, 'TRXPOS'),
    'TRXREG':     lambda data: disassemble_null(data, 'TRXREG'),
    'TRXDIR':     lambda data: disassemble_null(data, 'TRXDIR'),
    'HWREG':      lambda data: disassemble_null(data, 'HWREG'),

    # Other
    'SIGNAL':     lambda data: disassemble_null(data, 'SIGNAL'),
    'FINISH':     lambda data: disassemble_null(data, 'FINISH'),
    'LABEL':      lambda data: disassemble_null(data, 'LABEL'),
    'NOP':        lambda data: disassemble_null(data, 'NOP'),
}

def disassemble_command(register_name, hex_data):
    if register_name not in REVERSE_REGISTERS:
        return f"Unknown register name: {register_name}"

    register_code = REVERSE_REGISTERS[register_name]

    try:
        data = parse_hex(hex_data)
    except ValueError as e:
        return str(e)

    disassembly_function = DISASSEMBLY_FUNCTIONS[register_name]
    return disassembly_function(data)

def main():
    parser = argparse.ArgumentParser(description='ps2gfxdis')
    parser.add_argument('register_name', type=str, help='The name of the register (e.g., ALPHA_1)')
    parser.add_argument('data', type=str, help='The hexadecimal data associated with the register')

    args = parser.parse_args()

    result = disassemble_command(args.register_name, args.data)
    print(result)

if __name__ == "__main__":
    main()