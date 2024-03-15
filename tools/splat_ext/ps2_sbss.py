#!/usr/bin/env python3

# SPDX-FileCopyrightText: © 2024 AngheloAlf
# SPDX-License-Identifier: MIT

from __future__ import annotations

from splat.segtypes.common.bss import CommonSegBss

class PS2SegPs2_sbss(CommonSegBss):
    def get_linker_section(self) -> str:
        return ".sbss"

    @staticmethod
    def is_noload() -> bool:
        return True
