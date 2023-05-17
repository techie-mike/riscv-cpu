#include <cstdlib>
#include <stdlib.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "obj_dir/Vtop_single_cycle.h"
#include "verilatedos.h"

#define MAX_TIME 20
vluint64_t sim_time = 0;

int main(int argc, char** argv, char** env) {
    Vtop_single_cycle *top = new Vtop_single_cycle;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    top->trace(m_trace, 5);
    m_trace->open("waveform_single_cycle.vcd");
    while (sim_time < MAX_TIME) {
        top->CLK ^= 1;
        m_trace->dump(sim_time);
        sim_time++;
    }
    m_trace->close();
    delete top;
    exit(EXIT_SUCCESS);
}
