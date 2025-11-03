#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;
    
    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    
    int clk_of_nine = 0;
    // run simulation for many clock cycles
    for (i=0; i<300; i++) {

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++) {
            tfp->dump (2*i+clk);
            top->clk =!top->clk;
            top->eval ();
        }
        /*top->rst = (i<2) | (i==15);
        top->en = (i>4);
        */
        
        // Challenge : Stop counting for 3 cycles once the counter reaches 0x9, and then resume counting
        if (top->count == 0b00001001) {
            if(clk_of_nine == 0){
                clk_of_nine = i;
                top->en = 0;
            }
        
            else if (i == clk_of_nine + 2) {
                top->en = 1;
            }
        }   
        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}