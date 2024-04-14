#!/usr/bin/env python3

# SPDX-FileCopyrightText: © 2022-2024 AngheloAlf
# SPDX-License-Identifier: MIT

from __future__ import annotations

import argparse
import mapfile_parser
from pathlib import Path
import rabbitizer


def decodeInstruction(bytesDiff: bytes, mapFile: mapfile_parser.MapFile) -> str|None:
    word = (bytesDiff[3] << 24) | (bytesDiff[2] << 16) | (bytesDiff[1] << 8) | (bytesDiff[0] << 0)
    instr = rabbitizer.Instruction(word)
    immOverride = None

    if not instr.isValid():
        return None

    if instr.isJumpWithAddress():
        # Instruction is a function call (jal)

        # Get the embedded address of the function call
        symAddress = instr.getInstrIndexAsVram()

        # Search for the address in the mapfile
        symInfo = mapFile.findSymbolByVramOrVrom(symAddress)
        if symInfo is not None:
            # Use the symbol from the mapfile instead of a raw value
            immOverride = symInfo.symbol.name

    return instr.disassemble(immOverride=immOverride, extraLJust=-20)

def firstDiffMain():
    parser = argparse.ArgumentParser(description="Find the first difference(s) between the built ROM and the base ROM.")

    parser.add_argument("-c", "--count", type=int, default=5, help="find up to this many instruction difference(s)")
    parser.add_argument("-a", "--add-colons", action='store_true', help="Add colon between bytes" )

    args = parser.parse_args()

    buildFolder = Path("build")

    BUILTROM = buildFolder / f"SCPS_150.17"
    BUILTMAP = buildFolder / f"SCPS_150.17.map"

    EXPECTEDROM = "expected" / BUILTROM
    EXPECTEDMAP = "expected" / BUILTMAP

    mapfile_parser.frontends.first_diff.doFirstDiff(BUILTMAP, EXPECTEDMAP, BUILTROM, EXPECTEDROM, args.count, mismatchSize=True, addColons=args.add_colons, bytesConverterCallback=decodeInstruction, endian="little")

if __name__ == "__main__":
    firstDiffMain()
