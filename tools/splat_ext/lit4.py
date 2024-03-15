#!/usr/bin/env python3

from splat.segtypes.common.rodata import CommonSegRodata

class PS2SegLit4(CommonSegRodata):
    def get_linker_section(self) -> str:
        return ".lit4"

    def get_section_flags(self) -> str|None:
        return "wa"