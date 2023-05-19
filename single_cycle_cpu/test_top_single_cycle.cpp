#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <ios>
#include <iterator>
#include <ostream>
#include <stdlib.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "obj_dir/Vtop_single_cycle.h"
#include "obj_dir/Vram.h"
#include "obj_dir/Vtop_single_cycle_top_single_cycle.h"
#include "obj_dir/Vtop_single_cycle_ram.h"
#include "verilatedos.h"
#include <array>
#include <iostream>

#define MAX_TIME 20
vluint64_t sim_time = 0;
const uint32_t RISCV_INST_START = 0x1000;

int main(int argc, char** argv, char** env) {
    Vtop_single_cycle *top = new Vtop_single_cycle;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    top->trace(m_trace, 5);
    m_trace->open("waveform_single_cycle.vcd");

    // Load program in instruction memory
    std::array<uint32_t, 20 + RISCV_INST_START> init_inst_mem = {};
    std::array<uint32_t, 20> program = {
        0xffc4a303,  // lw x6, -4(x9)
        0xfc555555,
        2,
        3
    };
    auto iter_mem {init_inst_mem.begin()};
    iter_mem += RISCV_INST_START;
    std::copy(program.begin(), program.end(), iter_mem);
    std::copy(init_inst_mem.begin(), init_inst_mem.end(), top->top_single_cycle->inst_mem->mem);

    std::cerr << std::hex << (int)init_inst_mem[0x1000] << "\n";

    top->CLK = 0;
    while (sim_time < MAX_TIME) {
        top->CLK ^= 1;
        top->eval();
        m_trace->dump(sim_time);

        // Check instruction memory
        if (program.at(sim_time / 2) != top->top_single_cycle->INSTR) {
            std::cerr << program.at(sim_time) << " " << top->top_single_cycle->INSTR;
            std::cerr << "Program in instruction memory not equal with loaded program" << "\n";
        }
        sim_time++;
    }
    m_trace->close();
    delete top;
    exit(EXIT_SUCCESS);
}
