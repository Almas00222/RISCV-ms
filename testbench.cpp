#include "Vtop.h"            // Verilated top-module header for your CPU
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vtop___024root.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>

// -----------------------------------------------------------------------------
// Write Test Instructions to a Hex File
// -----------------------------------------------------------------------------
void writeInstructionsFile(const std::string &fname, const std::vector<uint32_t>& instrs) {
    std::ofstream out(fname);
    if (!out.is_open()) {
        std::cerr << "Cannot open " << fname << " for writing instructions!\n";
        exit(1);
    }
    // Write each instruction as 8 hex digits per line.
    for (auto instr : instrs) {
        out << std::hex
            << ((instr >> 28) & 0xF)
            << ((instr >> 24) & 0xF)
            << ((instr >> 20) & 0xF)
            << ((instr >> 16) & 0xF)
            << ((instr >> 12) & 0xF)
            << ((instr >>  8) & 0xF)
            << ((instr >>  4) & 0xF)
            << ( instr        & 0xF)
            << "\n";
    }
    // Fill remaining lines with NOP (0x00000013) to fill memory.
    for (int i = instrs.size(); i < 256; i++){
        out << "00000013\n";
    }
    out.close();
    std::cout << "Test instructions written to " << fname << "\n";
}

// -----------------------------------------------------------------------------
// Main Testbench
// -----------------------------------------------------------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    // 1) Define the test instruction sequence (addresses in increments of 4).
    //    This program tests add, sub, and, or, slt, addi, lw, sw, beq, jal.
    //    If successful, it writes the value 25 to memory address 100.
    std::vector<uint32_t> testInstrs = {
        0x00500113, // main:   addi x2, x0, 5      // x2 = 5
        0x00C00193, //         addi x3, x0, 12     // x3 = 12
        0xFF718393, //         addi x7, x3, -9     // x7 = 3   (12 - 9)
        0x0023E233, //         or   x4, x7, x2      // x4 = 7   (3 OR 5)
        0x0041F2B3, //         and  x5, x3, x4      // x5 = 4   (12 AND 7)
        0x004282B3, //         add  x5, x5, x4      // x5 = 11  (4 + 7)
        0x02728863, //         beq  x5, x7, end     // branch not taken
        0x0041A233, //         slt  x4, x3, x4      // x4 = 0   (12 < 7)
        0x00020463, //         beq  x4, x0, around  // branch taken
        0x00000293, //         addi x5, x0, 0      // should not execute
        0x0023A233, // around: slt  x4, x7, x2     // x4 = 1   (3 < 5)
        0x005203B3, //         add  x7, x4, x5      // x7 = 12  (1 + 11)
        0x402383B3, //         sub  x7, x7, x2      // x7 = 7   (12 - 5)
        0x0471AA23, //         sw   x7, 84(x3)      // store 7 at mem[96]
        0x06002103, //         lw   x2, 96(x0)      // x2 = 7   (read from mem[96])
        0x005104B3, //         add  x9, x2, x5      // x9 = 18  (7 + 11)
        0x008001EF, //         jal  x3, end         // jump to end, x3 becomes 0x44
        0x00100113, //         addi x2, x0, 1      // should not execute
        0x00910133, // end:    add  x2, x2, x9      // x2 = 25  (7 + 18)
        0x0221A023, //         sw   x2, 0x20(x3)    // store 25 at mem[100]
        0x00210063  // done:   beq  x2, x2, done    // infinite loop
    };

    // 2) Write instructions to file (e.g., "instructions.hex") used by your design.
    writeInstructionsFile("instructions.hex", testInstrs);

    // 3) Instantiate the CPU DUT.
    Vtop* top = new Vtop;
    
    // 4) Enable waveform tracing.
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("dump.vcd");
    
    // 5) Reset the DUT for a few cycles.
    top->rst = 1;
    top->clk = 0;
    for (int t = 0; t < 10; t++) {
        top->clk = !top->clk;
        top->eval();
        tfp->dump(t);
    }
    top->rst = 0;
    
    // 6) Run simulation for enough cycles to execute all instructions.
    for (int t = 10; t < 500; t++) {
        top->clk = !top->clk;
        top->eval();
        tfp->dump(t);
    }
    
    // 7) Golden comparison: Check that memory at address 100 holds 25.
    //    Assuming data memory is word-addressable and accessible as:
    //    top->rootp->top__DOT__data_mem[index]
    //    where index = address / 4.
    uint32_t memIndex = 104 / 4; // Convert byte address to word index.
    uint32_t memVal = top->rootp->top__DOT__mem__DOT__RAM[memIndex];
    
    bool pass = (memVal == 25);
    if (pass) {
        std::cout << "[PASS] Memory at address 100 contains 25.\n";
    } else {
        std::cout << "[FAIL] Memory at address 100 = 0x" 
                  << std::hex << memVal << " (expected 0x19, i.e., 25 in decimal).\n";
    }
    
    // 8) Cleanup.
    tfp->close();
    delete tfp;
    delete top;
    return 0;
}
