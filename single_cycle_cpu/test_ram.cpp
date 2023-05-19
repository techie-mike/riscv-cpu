#include <cstdlib>
#include <stdlib.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "obj_dir/Vram.h"
#include "verilatedos.h"

#define MAX_TIME 20
vluint64_t sim_time = 0;

int main(int argc, char** argv, char** env) {
    Vram *ram = new Vram;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    ram->trace(m_trace, 5);
    m_trace->open("waveform_ram_test.vcd");
    while (sim_time < MAX_TIME) {
        ram->CLK ^= 1;
        ram->A = (sim_time + 2) / 2;
        ram->WD = (sim_time + 1) / 2;
        ram->WE = sim_time % 2 + 1;
        ram->eval();
        m_trace->dump(sim_time);
        sim_time++;
    }
    m_trace->close();
    delete ram;
    exit(EXIT_SUCCESS);
}
