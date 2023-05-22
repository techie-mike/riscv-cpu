#!/bin/bash
set -ex

MODULE="top_single_cycle"
FILE="$MODULE.v"
TEST_FILE="test_$MODULE.cpp"
DEPENDES="pc.v ram.v regfile.v extend.v alu.v"

verilator --cc $FILE $DEPENDES --trace
verilator  --trace -cc $FILE --exe $TEST_FILE
make -C obj_dir/ -f Vtop_single_cycle.mk Vtop_single_cycle
./obj_dir/Vtop_single_cycle