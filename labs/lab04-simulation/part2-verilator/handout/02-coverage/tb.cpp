#include "verilated.h"
#include "Valu.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cassert>
#include <iomanip>

int main(int argc, char ** argv) {
    Verilated::commandArgs(argc, argv);
    
    auto dut = new Valu;

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 100; j++) {
            dut->funct = i;
            dut->in_a = rand();
            dut->in_b = rand();
            dut->eval();
        }
    }

    Verilated::threadContextp()->coveragep()->write("cov.dat");
    return 0;
}