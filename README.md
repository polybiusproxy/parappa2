# PaRappa the Rapper 2 Decompilation
![build](https://img.shields.io/github/actions/workflow/status/polybiusproxy/parappa2/build.yml?branch=main&label=build)
![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/total_progress.json)
![discord](https://img.shields.io/discord/302537923910303744?color=%235865F2&logo=discord&logoColor=%23FFFFFF)

<img src=".github/resources/transparent.png" style="margin:7px" align="right" width="20%" alt="PaRappa icon by pips">

A work-in-progress decompilation of [*PaRappa the Rapper 2*](https://en.wikipedia.org/wiki/PaRappa_the_Rapper_2) (パラッパラッパー2) for the PlayStation 2.<br>
We are currently targeting the July 12th NTSC-J prototype, but we aim to target the final NTSC-J/NTSC/PAL builds in the foreseeable future.<br><br>
For any questions, we have a home at the [PS1/PS2 Decompilation Discord server](https://discord.gg/VwCPdfbxgm) and the [PaRappa the Rapper Modding Community Discord server](https://discord.gg/xpvVnYd).

### Progress
#### EE main core
| Folder | Progress | Description
|------------|----------|------------
| `dbug` | ![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/dbug_progress.json) | Debug utilities (VRAM save, debug menus, etc.)
| `os` | ![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/os_progress.json) | OS utilities (threading, pad, memory, etc.)
| `iop_mdl` | ![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/iop_mdl_progress.json) | IOP module control routines
| `main` | ![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/main_progress.json) | Gameplay code (score logic, loading screen, etc.)
| `menu` | ![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/menu_progress.json) | Menu code (UI flow, Memory Card saving, etc.)
| `prlib` | ![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/prlib_progress.json) | Game engine (rendering, models/animations, etc.)
| `nalib` | ![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/nalib_progress.json) | Game engine utilities (vectors, matrices, packets)
| `src` | ![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/total_progress.json) | Total percentage

#### IOP modules
| Folder | Progress | Description
|------------|----------|------------
| `tapctrl` | N/A | Voice and sound effect playback
| `wavep2` | N/A | Music and asset streaming

#### VU1 microcode
| Folder | Progress | Description
|------------|----------|------------
| N/A | N/A | N/A

### Requirements setup
1. `sudo apt install gcc-mipsel-linux-gnu binutils-mips-linux-gnu ninja-build`
2. `sudo apt install python3-pip`
3. `python3 -m pip install -U -r requirements.txt`

*Note: For compiling, you need to add the `i386` architecture. For instructions on that please check out [this guide](https://askubuntu.com/questions/454253/how-to-run-32-bit-app-in-ubuntu-64-bit) if you're on Ubuntu.*

### Setup

1. Create a folder named `iso` in the root of the repo.
2. Extract `SCPS_150.17` from an ISO of the July 12th prototype and place it in said `iso` folder.
3. `./configure.py` (use `-c` to do a clean split.)
4. `ninja`

### asm-differ setup
1. After a successful build, create a folder named `expected`
2. Copy the `build` folder inside the `expected` folder.
3. `./tools/diff.py -mwo FUNCTION_NAME`

*Note: Every time you add a new symbol, you must do the asm-differ setup again.*
