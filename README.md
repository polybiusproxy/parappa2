# PaRappa the Rapper 2 Decompilation
![build](https://img.shields.io/github/actions/workflow/status/polybiusproxy/parappa2/build.yml?branch=main&label=build)
![progress](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/polybiusproxy/parappa2/main/progress/total_progress.json)
![discord](https://img.shields.io/discord/302537923910303744?color=%235865F2&logo=discord&logoColor=%23FFFFFF)

<img src=".github/resources/transparent.png" style="margin:7px" align="right" width="30%" alt="PaRappa icon by pips">

A work-in-progress decompilation of [*PaRappa the Rapper 2*](https://en.wikipedia.org/wiki/PaRappa_the_Rapper_2) (パラッパラッパー2) for the PlayStation 2.<br>
We are currently targeting the July 12th NTSC-J prototype, but we aim to target the final NTSC-J/NTSC/PAL builds in the foreseeable future.

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
