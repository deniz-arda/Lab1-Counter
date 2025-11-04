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
    
    int pause_counter = 0;  // Add counter for pause duration
    bool is_paused = false; // Add flag to track pause state
    // run simulation for many clock cycles
    for (i=0; i<300; i++) {

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++) {
            tfp->dump (2*i+clk);
            top->clk =!top->clk;
            top->eval ();
        }
        
        top->rst = (i<2) | (i==15);
        top->en = (i>4);
        
        // Stop counting for 3 cycles at 9
        if (top->count == 0x9 && !is_paused) {
            is_paused = true;
            pause_counter = 0;
        }
        
        if (is_paused) {
            top->en = 0;  // Disable counting
            pause_counter++;
            
            if (pause_counter >= 3) {
                is_paused = false;
                top->en = 1;  // Re-enable counting
            }
        }
        
        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}