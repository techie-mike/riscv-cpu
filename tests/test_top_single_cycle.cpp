#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <iterator>
#include <ostream>
#include <stdlib.h>

#include <stdio.h>
#include <array>
#include <iostream>

#include <verilated.h>
#include <verilated_vcd_c.h>
#include "verilatedos.h"

#include "obj_dir/Vtop_single_cycle.h"
#include "obj_dir/Vtop_single_cycle_top_single_cycle.h"
#include "obj_dir/Vtop_single_cycle_ram.h"
#include "obj_dir/Vtop_single_cycle_regfile.h"


#define MAX_TIME 20
vluint64_t sim_time = 0;

constexpr uint32_t RISCV_INST_START = 0x1000;

void DumpRegisters(const IData registers[]);
void WriteValueInMem(IData *mem, size_t pos, IData value);

void Test1();

int main(int argc, char** argv, char** env) {
    Test1();
    printf("\033[92m==================TESTS=PASSED==================\n");
    exit(EXIT_SUCCESS);
}

void DumpRegisters(const IData registers[])
{
    constexpr int HEIGHT = 11;
    constexpr int LENGTH = 32 / HEIGHT + 1;
    printf("------DUMP-REGISTERS-STATE------\n");
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < LENGTH; col++) {
            printf("[x%2d] = %-#10x (%-10d)\t", col * HEIGHT + row, registers[col * HEIGHT + row], registers[col * HEIGHT + row]);
        }
        printf("\n");
    }
    printf("------DUMP-REGISTERS-STATE------\n\n");

}

void Test1()
{
    // Test lw
    // --------
    // Inst: lw x6, -4(x9)
    // Init value: reg[x9] = 0x2004
    //             mem[0x2000] = 10
    // --------

    printf("==================RUNING=TEST=1==================\n");
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
        3,
        0x454545
    };
    auto iter_mem {init_inst_mem.begin()};
    iter_mem += RISCV_INST_START >> 2;
    std::copy(program.begin(), program.end(), iter_mem);
    std::copy(init_inst_mem.begin(), init_inst_mem.end(), top->top_single_cycle->inst_mem->mem);

    std::cerr << std::hex << (int)init_inst_mem[0x1000 >> 2] << "\n";
    std::cerr << std::hex << (int)top->top_single_cycle->inst_mem->mem[0x1000 >> 2] << "\n";

    top->top_single_cycle->data_mem->mem[0x2000] = 10;
    WriteValueInMem(top->top_single_cycle->data_mem->mem, 0x2000, 10);
    top->top_single_cycle->regfile->regs[9] = 0x2004;
    top->CLK = 0;
    while (sim_time < 4) {
        top->CLK ^= 1;
        top->eval();
        m_trace->dump(sim_time);

        // Check instruction memory
        if (program.at(sim_time / 2) != top->top_single_cycle->INSTR) {
            std::cerr << "Program in instruction memory not equal with loaded program!" << "\n";
            std::cerr << std::hex << program.at(sim_time) << " " << top->top_single_cycle->INSTR;
            exit(EXIT_FAILURE);
        }

        DumpRegisters(top->top_single_cycle->regfile->regs);
        sim_time++;
    }
    m_trace->close();
    assert(top->top_single_cycle->regfile->regs[6] == 10);
    delete top;
    printf("==================ENDED=TEST=1==================\n");
}

void WriteValueInMem(IData *mem, size_t pos, IData value)
{
    mem[pos >> 2] = value;
}