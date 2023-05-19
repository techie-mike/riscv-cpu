#!/bin/bash
set -x

verilator --cc ram.v --trace
verilator -Wall --trace -cc ram.v --exe test_ram.cpp
make -C obj_dir/ -f Vram.mk Vram
./obj_dir/Vram