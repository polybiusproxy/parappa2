SECTIONS
{
    __romPos = 0;
    _gp = 0x0;
    elf_header_ROM_START = __romPos;
    elf_header_VRAM = ADDR(.elf_header);
    .elf_header : AT(elf_header_ROM_START) SUBALIGN(16)
    {
        FILL(0x00000000);
        elf_header_DATA_START = .;
        elf_header_s = .;
        build/asm/data/elf_header.s.o(.data);
        elf_header_DATA_END = .;
        elf_header_DATA_SIZE = ABSOLUTE(elf_header_DATA_END - elf_header_DATA_START);
    }
    __romPos += SIZEOF(.elf_header);
    elf_header_ROM_END = __romPos;
    elf_header_VRAM_END = .;

    main_ROM_START = __romPos;
    main_VRAM = ADDR(.main);
    .main 0x100000 : AT(main_ROM_START) SUBALIGN(16)
    {
        FILL(0x00000000);
        main_TEXT_START = .;
        build/asm/unk.s.o(.text);
        main_TEXT_END = .;
        main_TEXT_SIZE = ABSOLUTE(main_TEXT_END - main_TEXT_START);
        main_DATA_START = .;
        vutext_s = .;
        build/asm/data/vutext.s.o(.data);
        unk_data__s = .;
        build/asm/data/unk.data.s.o(.data);
        vudata_s = .;
        build/asm/data/vudata.s.o(.data);
        gcc_except_table_s = .;
        build/asm/data/gcc_except_table.s.o(.data);
        unk_sections_s = .;
        build/asm/data/unk_sections.s.o(.data);
        main_DATA_END = .;
        main_DATA_SIZE = ABSOLUTE(main_DATA_END - main_DATA_START);
        main_RODATA_START = .;
        build/asm/data/unk.rodata.s.o(.rodata);
        main_RODATA_END = .;
        main_RODATA_SIZE = ABSOLUTE(main_RODATA_END - main_RODATA_START);
    }
    main_bss_VRAM = ADDR(.main_bss);
    .main_bss (NOLOAD) : SUBALIGN(16)
    {
        FILL(0x00000000);
        main_BSS_START = .;
        build/asm/unk.s.o(.bss);
        main_BSS_END = .;
        main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);
    }
    __romPos += SIZEOF(.main);
    __romPos = ALIGN(__romPos, 16);
    main_ROM_END = __romPos;
    main_VRAM_END = .;

    _29AA1C_ROM_START = __romPos;
    _29AA1C_VRAM = ADDR(._29AA1C);
    ._29AA1C : AT(_29AA1C_ROM_START) SUBALIGN(16)
    {
        FILL(0x00000000);
        _29AA1C_DATA_START = .;
        _29AA1C_s = .;
        build/asm/data/29AA1C.s.o(.data);
        _29AA1C_DATA_END = .;
        _29AA1C_DATA_SIZE = ABSOLUTE(_29AA1C_DATA_END - _29AA1C_DATA_START);
    }
    __romPos += SIZEOF(._29AA1C);
    _29AA1C_ROM_END = __romPos;
    _29AA1C_VRAM_END = .;

    /DISCARD/ :
    {
        *(*);
    }
}
