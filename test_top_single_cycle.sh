#!/bin/bash
set -ex

MODULE="top_single_cycle"
FILE="$MODULE.v"
TEST_FILE="test_$MODULE.cpp"
DEPENDES="pc.v"

verilator --cc $FILE $DEPENDES --trace
verilator -Wall --trace -cc $FILE --exe $TEST_FILE
make -C obj_dir/ -f Vtop_single_cycle.mk Vtop_single_cycle
./obj_dir/Vtop_single_cycle