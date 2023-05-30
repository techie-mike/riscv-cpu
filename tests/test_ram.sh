#!/bin/bash
set -x
PATH_SRC="../src"

verilator --cc "$PATH_SRC/ram.v" --trace
verilator -Wall --trace -cc "$PATH_SRC/ram.v" --exe test_ram.cpp
make -C obj_dir/ -f Vram.mk Vram
./obj_dir/Vram
