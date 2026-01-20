# cotm32

A hobbyist's honestly horrible design for a RISC-V CPU. Do not judge me.

## Description

This project builds a Verilated RV32I CPU simulator with an interactive SDL + ImGui user interface. The CPU is implemented as a standard 5-phased pipelined processor (IF, ID, EX, MEM, WB). The top module itself includes several peripherals, such as CLINT and terminal UART.

## Dependencies

### Tools

- CMake 3.16+
- C++17-compatible compiler
    - GCC or Clang recommended
- Verilator
    - For SystemVerilog to C++ translation
- RISC-V GCC toolchain
    - For building the bootloader
    - Specifically: `riscv32-unknown-elf-gcc`

### Libraries

- OpenGL Core 3.3+
- SDL2
    - For windowing and input
- fmt
- ImGui
    - Included as a vendored dependency (in `sim/third_party/imgui`)

## Installation

Before building the project, make sure dependencies are installed:

### Ubuntu / Debian:

```sh
sudo apt update
sudo apt install -y \
    cmake \
    g++ \
    verilator \
    libsdl2-dev \
    libfmt-dev \
    mesa-common-dev \
    libgl1-mesa-dev
```

For RISC-V GCC, either use a prebuilt toolchain:

```sh
sudo apt install -y gcc-riscv64-unknown-elf
```

or install one from [the official GitHub repository for the toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain/).

## Building

From the repository root:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

This produces the following in the `build` directory:

- `cotm32`: simulator executable with a graphical user interface
- `boot.elf`: bootloader executable (will be automatically loaded into the CPU's boot ROM on startup)

## Notes

- The project currently implements the following instruction sets:
    - RV32I (Base Integer Instruction Set)
    - M (Integer Multiplication and Division)
    - Zicsr (Control and Status Registers)
- The project currently runs entirely in M-mode. Future support for U- and S-mode is planned.
- The simulator uses OpenGL via SDL, and thus requires a working graphics stack.
- Running on Windows requires WSL (Windows Subsystem for Linux), along with a windowing setup that supports OpenGL (e.g. WSLg or X/Wayland).
