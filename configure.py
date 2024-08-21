#! /usr/bin/env python3

# SPDX-FileCopyrightText: Copyright 2024 polyproxy
# SPDX-License-Identifier: MIT

import argparse
import os
import shutil
import subprocess
import sys
import re

from   pathlib     import Path
from   typing      import Dict, List, Set, Union
from   collections import defaultdict

import ninja_syntax

import splat
import splat.scripts.split as split
from   splat.segtypes.linker_entry import LinkerEntry

ROOT = Path(__file__).parent
TOOLS_DIR = ROOT / "tools"

YAML_FILE = "config/pr2.proto.yaml"
BASENAME = "SCPS_150.17"
LD_PATH = f"{BASENAME}.ld"
ELF_PATH = f"build/{BASENAME}"
MAP_PATH = f"build/{BASENAME}.map"
PRE_ELF_PATH = f"build/{BASENAME}.elf"

COMMON_INCLUDES = "-Iinclude -Isrc -isystem include/sdk/ee -isystem include/sdk -isystem include/gcc -isystem include/gcc/gcc-lib"
COMPILER_DIR = f"{TOOLS_DIR}/cc/ee-gcc2.96/bin"

COMPILER_FLAGS     = "-O2 -G8 -gstabs"
COMPILER_FLAGS_CPP = "-O2 -G8 -x c++ -fno-exceptions -gstabs"

COMPILE_CMD = (
    f"{COMPILER_DIR}/ee-gcc -c {COMMON_INCLUDES} {COMPILER_FLAGS}"
)
COMPILE_CMD_CPP = (
    f"{COMPILER_DIR}/ee-gcc -c {COMMON_INCLUDES} {COMPILER_FLAGS_CPP}"
)

WIBO_VER = "0.6.11"

# CALCULATE PROGRESS TODO:
# python3 -m mapfile_parser progress build/SCPS_150.17.map asm asm/nonmatchings/

def exec_shell(command: List[str], stdout = subprocess.PIPE) -> str:
    ret = subprocess.run(
        command, stdout=stdout, stderr=subprocess.PIPE, text=True
    )
    return ret.stdout


def clean():
    if os.path.exists(".splache"):
        os.remove(".splache")
    if os.path.exists(".ninja_log"):
        os.remove(".ninja_log")
    if os.path.exists("build.ninja"):
        os.remove("build.ninja")
    if os.path.exists("SCPS_150.17.ld"):
        os.remove("SCPS_150.17.ld")
    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("build", ignore_errors=True)


# Possibly the worst thing I have ever written... removing the %gp_rel accesses like this...
# For some reason, the regex doesn't work here, and I'm sure it'll be easier to match these than
# debug what's going on here.
GP_HACK_FILENAME_TABLE = [
    "DrawCtrlMain.s",
    "WipeParaInDisp.s", "WipeParaInDispMove.s", "WipeParaOutDisp.s",
    "TsMENU_GetMapTimeState.s", "TsSetScene_Map.s", "TsMenu_Init.s", "TsRanking_Set.s", "MpCityHall_Flow.s", "TsOption_Flow.s"
]
GP_HACK_REPLACE_TABLE  = {
    # main/drawctrl.c
    "DrawCtrlMain.s":               [(26,  f'/* 167EC 001157EC 788B838F */   lw        $3, drawEventrec'),
                                     (29,  f'/* 167F8 001157F8 788B838F */  lw         $3, drawEventrec'),
                                     (31,  f'/* 167FC 001157FC 6C8B848F */  lw         $4, drawCurrentLine'),
                                     (49,  f'/* 1683C 0011583C 708B848F */   lw        $4, drawCurrentTime'),
                                     (74,  f'/* 16894 00115894 708B838F */  lw         $3, drawCurrentTime'),
                                     (149, f'/* 1699C 0011599C 708B848F */   lw        $4, drawCurrentTime'),
                                     (209, f'/* 16A70 00115A70 708B858F */  lw         $5, drawCurrentTime'),
                                     (226, f'/* 16AAC 00115AAC 708B858F */   lw        $5, drawCurrentTime'),
                                     (232, f'/* 16ABC 00115ABC 5C8B80AF */  sw         $0, dr_tap_req_num')],

    # main/wipe.c
    "WipeParaInDisp.s":             [(171, f'/* 1F13C 0011E13C AC8682AF */  sw         $2, wipe_end_flag'),
                                     (175, f'/* 1F148 0011E148 AC8682AF */  sw         $2, wipe_end_flag')],
    "WipeParaInDispMove.s":         [(121, f'/* 1F32C 0011E32C AC8682AF */  sw         $2, wipe_end_flag'),
                                     (125, f'/* 1F338 0011E338 AC8682AF */  sw         $2, wipe_end_flag')],
    "WipeParaOutDisp.s":            [(113, f'/* 1F504 0011E504 AC8682AF */   sw        $2, wipe_end_flag')],

    # menu/menusub.c
    "TsMENU_GetMapTimeState.s": [], # 13, 16, 20, 22, 25, 26, 30, 34, 77, 103
    "TsSetScene_Map.s": [], # 4
    "TsMenu_Init.s": [], # 4, 101, 114, 134, 143, 159, 171, 198, 200, 201, 202, 203, 205
    "TsRanking_Set.s": [], # 14, 35, 50
    "MpCityHall_Flow.s": [], # 19, 37, 53, 54, 55, 57, 58, 95, 96, 99, 103, 105, 142, 
                             # 159, 164, 165, 182, 185, 194, 198, 205, 246, 255, 258, 268, 272, 274,
                             # 276, 277, 287, 293, 296, 299, 456, 460, 483, 545, 550, 572, 580, 587,
                             # 591, 596, 615, 621, 637, 658, 662, 673, 675, 679, 681, 693, 698, 699,
                             # 703, 705, 707, 741, 743, 759, 760, 780, 813, 815, 821, 843, 845, 854,
                             # 856, 860, 862, 870, 873, 877, 879, 883, 888, 897, 904, 915, 920, 921,
                             # 925, 929, 931, 932, 937, 939, 945, 971, 991, 1002, 1009, 1011, 1016,
                             # 1022, 1028, 1041, 1045, 1060, 1062, 1066, 1074, 1075, 1077, 1080
    "TsOption_Flow.s": [] # 17, 28, 29, 30, 32, 253, 301, 302, 303, 304

}

def gp_hack(filepath):
    filename = os.path.basename(filepath)
    if filename in GP_HACK_FILENAME_TABLE:
        replacements = GP_HACK_REPLACE_TABLE.get(filename, [])

        print(f"(HACK) Removing %gp_rel references on problematic file \"{filename}\"")

        with open(filepath, "r") as file:
            lines = file.readlines()

        for line_number, new_line in replacements:
            if 0 <= line_number < len(lines):
                lines[line_number] = new_line + "\n"
        
        with open(filepath, "w") as file:
            file.writelines(lines)

gp_access_pattern = re.compile(r'%(gp_rel)\(([^)]+)\)\(\$28\)') # Pattern for removing %gp_rel accesses
gp_add_pattern = re.compile(r'addiu\s+(\$\d+), \$28, %gp_rel\(([^)]+)\)') # Pattern for replacing %gp_rel additions
def remove_gprel():
    for root, dirs, files in os.walk("asm/nonmatchings/"):
        for filename in files:
            filepath = os.path.join(root, filename)

            gp_hack(filepath) # Hopefully I can get rid of this!

            with open(filepath, "r") as file:
                content = file.read()

            # Search for any %gp_rel access
            # INSTR REG, %gp_rel(SYMBOL)($28) -> INSTR REG, SYMBOL
            if re.search(gp_access_pattern, content):
                # Reference found, remove
                updated_content = re.sub(gp_access_pattern, r'\2', content)

                # Write the updated content back to the file
                with open(filepath, "w") as file:
                    file.write(updated_content)
            
            # Search for any %gp_rel additions
            # addiu REG, $28, %gp_rel(SYMBOL) -> la REG, SYMBOL
            if re.search(gp_add_pattern, content):
                # Reference found, replace
                updated_content = re.sub(gp_add_pattern, r'la \1, \2', content)

                # Write the updated content back to the file
                with open(filepath, "w") as file:
                    file.write(updated_content)

EUC_HACK_FILENAME_TABLE = ["TsDrawUPacket.s", "_P3MC_SetBrowsInfo.s"]
def eucjp_convert():
    for root, dirs, files in os.walk("asm/nonmatchings/"):
        for filename in files:
            if filename.startswith("D_") or filename in EUC_HACK_FILENAME_TABLE:
                filepath = os.path.join(root, filename)

                if filename in EUC_HACK_FILENAME_TABLE:
                    print(f"(HACK) Converting {filename}")

                with open(filepath, "r", encoding="utf-8") as file:
                    content = file.read()

                eucjp_content = content.encode("euc-jp").decode("euc-jp")

                with open(filepath, "w", encoding="euc-jp") as file:
                    file.write(eucjp_content)

gp_analysis_pattern = re.compile(r'/\* gp_rel: \((\w+)\) \*/')
def perform_gp_analysis():
    references = defaultdict(lambda: defaultdict(list))
    for root, dirs, files in os.walk("asm/nonmatchings/"):
        for filename in files:
            filepath = os.path.join(root, filename)

            with open(filepath, "r") as file:
                for line_num, line in enumerate(file, 1):
                    analysis_match = gp_analysis_pattern.search(line)
                    if analysis_match:
                        variable_name = analysis_match.group(1)
                        references[filename][variable_name].append(line_num)

    max_line_length = 48
    border_prefix = "---- "
    space_after_filename = 1

    available_length = max_line_length - len(border_prefix) - space_after_filename
    
    print()
    print("---------- @parappa2, gp_rel analysis ----------")
    for file in sorted(references.keys()):
        vars_dict = references[file]
        truncated_filename = file[:available_length]
        dashes_length = max_line_length - len(border_prefix) - len(truncated_filename) - space_after_filename
        border_line = f"{border_prefix}{truncated_filename} {'-' * dashes_length}"
        print(border_line)
        for variable_name in sorted(vars_dict.keys()):
            lines = vars_dict[variable_name]
            ref_count = len(lines)
            print(f"Variable: {variable_name} (references: {ref_count})")
            print("Locations:")
            for line_num in lines:
                print(f"    - {file}:{line_num}")
            print()
        print("------------------------------------------------")
    print("-------------- gp_rel analysis end -------------")
    print("------------------------------------------------")

def write_permuter_settings():
    with open("permuter_settings.toml", "w") as f:
        f.write(
            f"""compiler_command = "tools/cc/ee-gcc2.96/bin/ee-gcc -c -Iinclude -Iinclude/sdk/ee -Iinclude/gcc -Iinclude/gcc/gcc-lib -O2 -G8 -gstabs -D__GNUC__"
assembler_command = "mips-linux-gnu-as -march=r5900 -mabi=eabi -Iinclude"
compiler_type = "gcc"

[preserve_macros]

[decompme.compilers]
"tools/cc/ee-gcc2.96/bin/ee-gcc" = "ee-gcc2.9-991111-01"
"""
        )


def build_stuff(linker_entries: List[LinkerEntry]):
    built_objects: Set[Path] = set()

    def build(
        object_paths: Union[Path, List[Path]],
        src_paths: List[Path],
        task: str,
        variables: Dict[str, str] = {},
        implicit_outputs: List[str] = [],
    ):
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        object_strs = [str(obj) for obj in object_paths]

        for object_path in object_paths:
            if object_path.suffix == ".o":
                built_objects.add(object_path)
            ninja.build(
                outputs=object_strs,
                rule=task,
                inputs=[str(s) for s in src_paths],
                variables=variables,
                implicit_outputs=implicit_outputs,
            )

    ninja = ninja_syntax.Writer(open(str(ROOT / "build.ninja"), "w"), width=9999)

    # Rules
    cross = "mips-linux-gnu-"
    ld_args = f"-EL -T config/undefined_syms_auto.txt -T config/undefined_funcs_auto.txt -T config/undefined_syms.txt -Map $mapfile -T $in -o $out"

    ninja.rule(
        "as",
        description="as $in",
        command=f"cpp {COMMON_INCLUDES} $in -o - | iconv -f=UTF-8 -t=EUC-JP $in | {cross}as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o $out && python3 tools/buildtools/elf_patcher.py $out gas",
    )

    ninja.rule(
        "cpp",
        description="cpp $in",
        command=f"{COMPILE_CMD_CPP} $in -o $out && {cross}strip $out -N dummy-symbol-name",
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=f"{COMPILE_CMD} $in -o $out && {cross}strip $out -N dummy-symbol-name",
    )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{cross}ld {ld_args}",
    )

    ninja.rule(
        "sha1sum",
        description="sha1sum $in",
        command="sha1sum -c $in && touch $out",
    )

    ninja.rule(
        "elf",
        description="elf $out",
        command=f"{cross}objcopy $in $out -O binary",
    )

    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if isinstance(seg, splat.segtypes.common.asm.CommonSegAsm) or isinstance(
            seg, splat.segtypes.common.data.CommonSegData
        ):
            build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.cpp.CommonSegCpp):
            build(entry.object_path, entry.src_paths, "cpp")
        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            build(entry.object_path, entry.src_paths, "cc")
        elif isinstance(seg, splat.segtypes.common.databin.CommonSegDatabin) or isinstance(seg, splat.segtypes.common.rodatabin.CommonSegRodatabin):
            build(entry.object_path, entry.src_paths, "as")
        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)

    ninja.build(
        PRE_ELF_PATH,
        "ld",
        LD_PATH,
        implicit=[str(obj) for obj in built_objects],
        variables={"mapfile": MAP_PATH},
    )

    ninja.build(
        ELF_PATH,
        "elf",
        PRE_ELF_PATH,
    )

    ninja.build(
        ELF_PATH + ".ok",
        "sha1sum",
        "checksum.sha1",
        implicit=[ELF_PATH],
    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "-c",
        "--clean",
        help="Clean extraction and build artifacts",
        action="store_true",
    )
    parser.add_argument(
        "-csrc",
        "--cleansrc",
        help="Clean the 'src' folder",
        action="store_true",
    )
    parser.add_argument(
        "-nogp",
        "--no-gprel-removing",
        help="Do not remove gp_rel references on the disassembly",
        action="store_true",
        default=True
    )
    parser.add_argument(
        "-noeuc",
        "--no-eucjp-converting",
        help="Do not convert to EUC-JP the disassembly strings",
        action="store_true",
    )
    parser.add_argument(
        "-gpanalysis",
        "--gp-analysis",
        help="Analyzes every use of gp_rel, for aiding the splitting of a file",
        action="store_true",
    )
    args = parser.parse_args()

    if args.clean:
        clean()
    
    if args.cleansrc:
        shutil.rmtree("src", ignore_errors=True)

    split.main([YAML_FILE], modes="all", verbose=False)

    linker_entries = split.linker_writer.entries

    build_stuff(linker_entries)

    write_permuter_settings()

    # Remove the %gp_rel references
    if not args.no_gprel_removing:
        remove_gprel()

    # Only to be performed with a clean build
    if args.gp_analysis:
        perform_gp_analysis()

    if not args.no_eucjp_converting:
        eucjp_convert()

    if not os.path.isfile("compile_commands.json"):
        exec_shell(["ninja", "-t", "compdb"], open("compile_commands.json", "w"))
