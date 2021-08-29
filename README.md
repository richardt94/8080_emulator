# 8080_emulator
Emulator for an 8080 _Space Invaders_ arcade machine, now in C.

Currently supports emulation of a single-player _Space Invaders_ game. Press C to insert a coin, then Enter to start the game, left and right to move and space to shoot.
## Installation
Start by installing the dependencies. Building from source requires [SDL2](https://www.libsdl.org/) and [CMake](https://cmake.org/). Additionally, the test suite for the CPU requires [Check](https://libcheck.github.io/check/). The CMake install script will automatically detect whether SDL2 and Check are available on your system.

Once you have installed these, you can build and install from the root directory of the repository using:
```
mkdir build && cd build
cmake ..
make
sudo make install
```
These steps will be slightly different if you're installing on a system that uses a different build system to Make.

## Usage
If SDL2 was installed properly, the build steps will install two programs, `emu8080` and `disassemble`. `emu8080` takes a list of files as arguments, which are loaded sequentially into the emulated memory and executed (e.g. `invaders.h`, `invaders.g`, `invaders.f` and `invaders.e` from the _Space Invaders_ ROM). `disassemble` takes a binary executable for the 8080 and translates the instructions to assembly language using the mnemonics from the Intel 8080 Assembly Language Programming Manual.

## Notes
The CPU emulation passes all of the common Intel 8080 test binaries I could find. This includes `CPUDIAG.BIN` (Microcosm Associates CPU diagnostics), `CPUTEST.COM` 
(Diagnostics II V1.2) and `8080EX1.COM` (the 8080 exerciser with CRC values for the Russian KR580VM80A clone). There is a simple shell for the emulator in `tests/cpudiag_shell.c` which catches and emulates text output routines from the CP/M operating system, as all of these tests were originally written for CP/M. You'll need to find these test ROMS, and the _Space Invaders_ ROM itself, on your own as I am unsure of their copyright status and will not redistribute them.

The emulator displays each pixel from the framebuffer as a 3x3 block on the host machine's screen - this is because modern screens tend to have a lot more pixels than _Space Invaders_ did.
