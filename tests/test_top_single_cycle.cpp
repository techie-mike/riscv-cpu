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

constexpr uint32_t RISCV_INST_START = 0x1000;

void DumpRegisters(const IData registers[], const IData pc);
void WriteValueInMem(IData *mem, size_t pos, IData value);
int32_t ReadValueInMem(IData *mem, size_t pos);

void Test1();
void TestSW();
void TestOR();
void TestBEQ();
void TestSmallBench();

int main(int argc, char** argv, char** env) {
    Test1();
    TestSW();
    TestOR();
    TestBEQ();
    printf("\033[92m==================TESTS=PASSED==================\n");
    exit(EXIT_SUCCESS);
}

void DumpRegisters(const IData registers[], const IData pc)
{
    constexpr int HEIGHT = 11;
    constexpr int LENGTH = 32 / HEIGHT + 1;
    printf("------DUMP-REGISTERS-STATE------\n");
    printf("[pc]  = %#x\n", pc);
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
    // Result: reg[x6] = 10
    // --------

    printf("==================RUNING=TEST=1==================\n");
    vluint64_t sim_time = 0;
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
    top->top_single_cycle->REG_WRITE = 1;
    top->top_single_cycle->IMM_SRC = 0;
    top->top_single_cycle->MEM_WR = 0;
    top->top_single_cycle->ALU_SRC = 1;
    top->top_single_cycle->RESULT_SRC = 1;
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

        DumpRegisters(top->top_single_cycle->regfile->regs, top->top_single_cycle->PC);
        sim_time++;
    }
    m_trace->close();
    assert(top->top_single_cycle->regfile->regs[6] == 10);
    delete top;
    printf("==================ENDED=TEST=1==================\n");
}

void TestSW()
{
    // Test lw
    // --------
    // Inst: lw x6, -4(x9)
    //       sw x6, 8(x9)
    // Init value: reg[x9] = 0x2004
    //             mem[0x2000] = 10
    // Result: mem[0x200C] = 10
    // --------

    printf("==================RUNING=TESTSW==================\n");
    vluint64_t sim_time = 0;
    Vtop_single_cycle *top = new Vtop_single_cycle;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    top->trace(m_trace, 5);
    m_trace->open("waveform_single_cycle.vcd");

    // Load program in instruction memory
    std::array<uint32_t, 20 + RISCV_INST_START> init_inst_mem = {};
    std::array<uint32_t, 20> program = {
        0xffc4a303,  // lw x6, -4(x9)
        0x0064A423,  // sw x6,  8(x9)
    };
    auto iter_mem {init_inst_mem.begin()};
    iter_mem += RISCV_INST_START >> 2;
    std::copy(program.begin(), program.end(), iter_mem);
    std::copy(init_inst_mem.begin(), init_inst_mem.end(), top->top_single_cycle->inst_mem->mem);

    // std::cerr << std::hex << (int)init_inst_mem[0x1000 >> 2] << "\n";
    // std::cerr << std::hex << (int)top->top_single_cycle->inst_mem->mem[0x1000 >> 2] << "\n";

    WriteValueInMem(top->top_single_cycle->data_mem->mem, 0x2000, 10);
    top->top_single_cycle->regfile->regs[9] = 0x2004;
    top->top_single_cycle->REG_WRITE = 1;
    top->top_single_cycle->IMM_SRC = 0;
    top->top_single_cycle->MEM_WR = 0;
    top->top_single_cycle->ALU_SRC = 1;
    top->top_single_cycle->RESULT_SRC = 1;
    top->CLK = 0;

    while (sim_time < 6) {
        top->CLK ^= 1;
        if (sim_time >= 2) {
            top->top_single_cycle->MEM_WR = 1;
            top->top_single_cycle->IMM_SRC = 1;
        }

        top->eval();
        m_trace->dump(sim_time);

        // Check instruction memory
        if (program.at(sim_time / 2) != top->top_single_cycle->INSTR) {
            std::cerr << "Program in instruction memory not equal with loaded program!" << "\n";
            std::cerr << std::hex << program.at(sim_time) << " " << top->top_single_cycle->INSTR;
            exit(EXIT_FAILURE);
        }

        if (sim_time % 2 == 0) {
            DumpRegisters(top->top_single_cycle->regfile->regs, top->top_single_cycle->PC);
        }
        sim_time++;
    }
    m_trace->close();
    assert(ReadValueInMem(top->top_single_cycle->data_mem->mem, 0x200C) == 10);
    delete top;
    printf("==================ENDED=TESTSW==================\n");
}

void TestOR()
{
    // Test or
    // --------
    // Inst: lw x6, -4(x9)
    //       sw x6, 8(x9)
    //       or x4, x5, x6
    // Init value: reg[x9] = 0x2004
    //             reg[x5] = 6
    //             mem[0x2000] = 10
    // Result: reg[x4] = 14
    // --------

    printf("==================RUNING=TESTOR==================\n");
    vluint64_t sim_time = 0;
    Vtop_single_cycle *top = new Vtop_single_cycle;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    top->trace(m_trace, 5);
    m_trace->open("waveform_single_cycle.vcd");

    // Load program in instruction memory
    std::array<uint32_t, 20 + RISCV_INST_START> init_inst_mem = {};
    std::array<uint32_t, 20> program = {
        0xffc4a303,  // lw x6, -4(x9)
        0x0064A423,  // sw x6,  8(x9)
        0x0062E233,  // or x4, x5, x6
    };
    auto iter_mem {init_inst_mem.begin()};
    iter_mem += RISCV_INST_START >> 2;
    std::copy(program.begin(), program.end(), iter_mem);
    std::copy(init_inst_mem.begin(), init_inst_mem.end(), top->top_single_cycle->inst_mem->mem);

    // std::cerr << std::hex << (int)init_inst_mem[0x1000 >> 2] << "\n";
    // std::cerr << std::hex << (int)top->top_single_cycle->inst_mem->mem[0x1000 >> 2] << "\n";

    WriteValueInMem(top->top_single_cycle->data_mem->mem, 0x2000, 10);
    top->top_single_cycle->regfile->regs[9] = 0x2004;
    top->top_single_cycle->regfile->regs[5] = 6;
    top->top_single_cycle->REG_WRITE = 1;
    top->top_single_cycle->IMM_SRC = 0;
    top->top_single_cycle->MEM_WR = 0;
    top->top_single_cycle->ALU_SRC = 1;
    top->top_single_cycle->RESULT_SRC = 1;
    top->CLK = 0;

    while (sim_time < 7) {
        top->CLK ^= 1;
        if (sim_time == 2 || sim_time == 3) {
            top->top_single_cycle->MEM_WR = 1;
            top->top_single_cycle->IMM_SRC = 1;
        }

        if (sim_time == 4 || sim_time == 5) {
            top->top_single_cycle->MEM_WR = 0;
            top->top_single_cycle->REG_WRITE = 1;
            top->top_single_cycle->ALU_SRC = 0;
            top->top_single_cycle->RESULT_SRC = 0;
            top->top_single_cycle->ALU_CONTROL = 3;
        }

        top->eval();
        m_trace->dump(sim_time);

        // Check instruction memory
        if (program.at(sim_time / 2) != top->top_single_cycle->INSTR) {
            std::cerr << "Program in instruction memory not equal with loaded program!" << "\n";
            std::cerr << std::hex << program.at(sim_time) << " " << top->top_single_cycle->INSTR;
            exit(EXIT_FAILURE);
        }

        if (sim_time % 2 == 0) {
            DumpRegisters(top->top_single_cycle->regfile->regs, top->top_single_cycle->PC);
        }
        sim_time++;
    }
    m_trace->close();
    assert(top->top_single_cycle->regfile->regs[4] == 14);
    delete top;
    printf("==================ENDED=TESTOR==================\n");
}

void TestBEQ()
{
    // Test beq
    // --------
    // Inst: L7: lw x6, -4(x9)
    //           sw x6, 8(x9)
    //           or x4, x5, x6
    //           beq x4, x4, L7
    // Init value: reg[x9] = 0x2004
    //             reg[x5] = 6
    //             mem[0x2000] = 10
    // Result: pc = 0x1000
    // --------

    printf("==================RUNING=TEST=BEQ==================\n");
    vluint64_t sim_time = 0;
    Vtop_single_cycle *top = new Vtop_single_cycle;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    top->trace(m_trace, 5);
    m_trace->open("waveform_single_cycle.vcd");

    // Load program in instruction memory
    std::array<uint32_t, 20 + RISCV_INST_START> init_inst_mem = {};
    std::array<uint32_t, 20> program = {
        0xffc4a303,  // L7: lw x6, -4(x9)
        0x0064A423,  //     sw x6,  8(x9)
        0x0062E233,  //     or x4, x5, x6
        0xfe420ae3,  //     beq x4, x4, L7
    };
    auto iter_mem {init_inst_mem.begin()};
    iter_mem += RISCV_INST_START >> 2;
    std::copy(program.begin(), program.end(), iter_mem);
    std::copy(init_inst_mem.begin(), init_inst_mem.end(), top->top_single_cycle->inst_mem->mem);

    WriteValueInMem(top->top_single_cycle->data_mem->mem, 0x2000, 10);
    top->top_single_cycle->regfile->regs[9] = 0x2004;
    top->top_single_cycle->regfile->regs[5] = 6;
    top->top_single_cycle->REG_WRITE = 1;
    top->top_single_cycle->IMM_SRC = 0;
    top->top_single_cycle->MEM_WR = 0;
    top->top_single_cycle->ALU_SRC = 1;
    top->top_single_cycle->RESULT_SRC = 1;
    top->top_single_cycle->PC_SRC = 0;
    top->CLK = 0;

    while (sim_time < 9) {
        top->CLK ^= 1;
        if (sim_time == 2 || sim_time == 3) {
            top->top_single_cycle->MEM_WR = 1;
            top->top_single_cycle->IMM_SRC = 1;
        }

        if (sim_time == 4 || sim_time == 5) {
            top->top_single_cycle->MEM_WR = 0;
            top->top_single_cycle->REG_WRITE = 1;
            top->top_single_cycle->ALU_SRC = 0;
            top->top_single_cycle->RESULT_SRC = 0;
            top->top_single_cycle->ALU_CONTROL = 3;
        }

        if (sim_time == 6 || sim_time == 7) {
            top->top_single_cycle->IMM_SRC = 0b10;
            top->top_single_cycle->PC_SRC = 1;
        }

        top->eval();
        m_trace->dump(sim_time);

        if (sim_time % 2 == 0) {
            DumpRegisters(top->top_single_cycle->regfile->regs, top->top_single_cycle->PC);
        }
        sim_time++;
    }
    m_trace->close();
    assert(top->top_single_cycle->PC == 0x1000);
    delete top;
    printf("==================ENDED=TEST=BEQ==================\n");
}

void TestSmallBench()
{
    // Test cycle
    // --------
    // Inst: L7: lw x6, -4(x9)
    //           sw x6, 8(x9)
    //           or x4, x5, x6
    //           beq x4, x4, L7
    // Init value: reg[x9] = 0x2004
    //             reg[x5] = 6
    //             mem[0x2000] = 10
    // --------

    printf("==================RUNING=TEST=SMALL=BENCH==================\n");
    vluint64_t sim_time = 0;
    Vtop_single_cycle *top = new Vtop_single_cycle;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    top->trace(m_trace, 5);
    m_trace->open("waveform_single_cycle.vcd");

    // Load program in instruction memory
    std::array<uint32_t, 20 + RISCV_INST_START> init_inst_mem = {};
    std::array<uint32_t, 20> program = {
        0xffc4a303,  // L7: lw x6, -4(x9)
        0x0064A423,  //     sw x6,  8(x9)
        0x0062E233,  //     or x4, x5, x6
        0xfe420ae3,  //     beq x4, x4, L7
    };
    auto iter_mem {init_inst_mem.begin()};
    iter_mem += RISCV_INST_START >> 2;
    std::copy(program.begin(), program.end(), iter_mem);
    std::copy(init_inst_mem.begin(), init_inst_mem.end(), top->top_single_cycle->inst_mem->mem);

    WriteValueInMem(top->top_single_cycle->data_mem->mem, 0x2000, 10);
    top->top_single_cycle->regfile->regs[9] = 0x2004;
    top->top_single_cycle->regfile->regs[5] = 6;
    top->CLK = 0;

    while (sim_time < 9) {
        top->CLK ^= 1;
        top->eval();
        m_trace->dump(sim_time);

        if (sim_time % 2 == 0) {
            DumpRegisters(top->top_single_cycle->regfile->regs, top->top_single_cycle->PC);
        }
        sim_time++;
    }
    m_trace->close();
    assert(top->top_single_cycle->PC == 0x1000);
    delete top;
    printf("==================RUNING=TEST=SMALL=BENCH==================\n");
}

void WriteValueInMem(IData *mem, size_t pos, IData value)
{
    mem[pos >> 2] = value;
}

int32_t ReadValueInMem(IData *mem, size_t pos)
{
    return mem[pos >> 2];
}
