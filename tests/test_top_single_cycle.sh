#!/bin/bash
set -ex
PATH_SRC="../src"

MODULE="top_single_cycle"
FILE="$PATH_SRC/$MODULE.v"
TEST_FILE="test_$MODULE.cpp"
DEPENDES="
$PATH_SRC/pc.v     \
$PATH_SRC/ram.v     \
$PATH_SRC/regfile.v \
$PATH_SRC/extend.v  \
$PATH_SRC/alu.v"

verilator --cc $FILE $DEPENDES --trace "-I$PATH_SRC"
verilator  --trace -cc $FILE --exe $TEST_FILE "-I$PATH_SRC"
make -C obj_dir/ -f Vtop_single_cycle.mk Vtop_single_cycle
./obj_dir/Vtop_single_cycle