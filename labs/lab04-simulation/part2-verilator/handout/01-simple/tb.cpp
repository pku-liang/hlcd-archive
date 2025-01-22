#include "verilated.h"
#include "Valu.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cassert>
#include <iomanip>

int main(int argc, char ** argv) {
    Verilated::commandArgs(argc, argv);

    auto dut = new Valu;

    Verilated::traceEverOn(true);
    auto tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open("trace.vcd");

    uint64_t time = 0;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 100; j++) {
            dut->funct = i;
            dut->in_a = rand();
            dut->in_b = rand();
            dut->eval();
            tfp->dump(time++);
            assert(dut->out == dut->gold);
        }
    }

    tfp->close();
    return 0;
}
