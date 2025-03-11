# RISCV-ms

# RISC-V Multicycle Processor

This project implements a 32-bit RISC-V multicycle processor written in Verilog. The design is inspired by "Digital Design and Computer Architecture RISC-V Edition" and supports a subset of the RV32I base instruction set. The processor is designed with multiple stages (fetch, decode, execute) to efficiently handle instruction execution and has been verified using a comprehensive RISC-V assembly test program.

## Features

- **32-bit Multicycle Architecture:** Implements multiple stages to execute instructions efficiently.
- **Supported Instructions:**
  - **Arithmetic:** `add`, `sub`, `addi`
  - **Logical:** `and`, `or`
  - **Comparison:** `slt`
  - **Memory Operations:** `lw`, `sw`
  - **Control Flow:** `beq`, `jal`
- **Verification:** The processor is tested with a RISC-V assembly program that executes a series of instructions to confirm correct operation.

## Test Program

The provided RISC-V assembly program performs a series of operations. If the processor functions correctly, it will eventually write the value `25` to memory address `100`. Below is the test program along with a description of each instruction:

| Label   | Instruction                | Description                                  | Address | Machine Code  |
|---------|----------------------------|----------------------------------------------|---------|---------------|
| main:   | `addi x2, x0, 5`           | Set `x2` to 5                                | 0       | 00500113      |
|         | `addi x3, x0, 12`          | Set `x3` to 12                               | 4       | 00C00193      |
|         | `addi x7, x3, -9`          | Set `x7` to `x3 - 9` (i.e., 12 - 9 = 3)        | 8       | FF718393      |
|         | `or x4, x7, x2`            | Compute `x4 = x7 OR x2` (3 OR 5 = 7)           | C       | 0023E233      |
|         | `and x5, x3, x4`           | Compute `x5 = x3 AND x4` (12 AND 7 = 4)        | 10      | 0041F2B3      |
|         | `add x5, x5, x4`           | Update `x5` to `x5 + x4` (4 + 7 = 11)          | 14      | 004282B3      |
|         | `beq x5, x7, end`          | Branch if `x5 == x7` (should not be taken)     | 18      | 02728863      |
|         | `slt x4, x3, x4`           | Set `x4` if `x3 < x4` (12 < 7 = false → 0)     | 1C      | 0041A233      |
|         | `beq x4, x0, around`       | Branch to label `around` if `x4 == 0` (taken)  | 20      | 00020463      |
|         | `addi x5, x0, 0`           | Should not be executed                       | 24      | 00000293      |
| around: | `slt x4, x7, x2`           | Set `x4` if `x7 < x2` (3 < 5 = true → 1)       | 28      | 0023A233      |
|         | `add x7, x4, x5`           | Compute `x7 = x4 + x5` (1 + 11 = 12)           | 2C      | 005203B3      |
|         | `sub x7, x7, x2`           | Compute `x7 = x7 - x2` (12 - 5 = 7)            | 30      | 40238383      |
|         | `sw x7, 84(x3)`            | Store `x7` (7) at memory location `x3+84` ([96]) | 34      | 0471AA23      |
|         | `lw x2, 96(x0)`            | Load value from memory address 96 into `x2` (7) | 38      | 06002103      |
|         | `add x9, x2, x5`           | Compute `x9 = x2 + x5` (7 + 11 = 18)           | 3C      | 005104B3      |
|         | `jal x3, end`              | Jump to label `end` (store return address in `x3`) | 40   | 008001EF      |
|         | `addi x2, x0, 1`           | Should not be executed                       | 44      | 00100113      |
| end:    | `add x2, x2, x9`           | Compute `x2 = x2 + x9` (7 + 18 = 25)           | 48      | 00910133      |
|         | `sw x2, 0x20(x3)`          | Store `x2` (25) at memory location `x3+0x20` ([100]) | 4C  | 0221A023      |
| done:   | `beq x2, x2, done`         | Infinite loop to end the program             | 50      | 00210063      |

When the processor is functioning correctly, the final result should be that memory address `100` contains the value `25`.

## Running the Simulation

Follow these steps to compile, simulate, and verify the processor:

1. **Setup Environment:**
   - **Verilator:** Ensure that Verilator is installed. Instructions for installation can be found on [Verilator's website](https://www.veripool.org/verilator) or via your system package manager.
   - **GTKWave:** Install GTKWave for waveform analysis to inspect the internal signal transitions.

2. **Compile the Verilog Code:**
   - Use Verilator to compile your design. For example:
     ```bash
     verilator -Wall --cc your_top_module.v --exe testbench.cpp
     ```
   - Replace `your_top_module.v` with the main Verilog file of your processor.

3. **Build the Simulation:**
   - Navigate to the generated `obj_dir` directory and compile the simulation:
     ```bash
     cd obj_dir
     make -j -f Vyour_top_module.mk
     ```
   - Replace `your_top_module` with the appropriate top module name.

4. **Run the Simulation:**
   - Execute the simulation:
     ```bash
     ./Vyour_top_module
     ```
   - Monitor the console output to check the progression of the test program.

5. **Analyze Waveforms:**
   - If your simulation generates a VCD (Value Change Dump) file, open it with GTKWave:
     ```bash
     gtkwave dump.vcd
     ```
   - Verify that the value `25` is stored at memory address `100` by inspecting the corresponding signals.

## Code Structure

- **Verilog Files:** Contain the implementation of the multicycle RISC-V processor.
- **Testbench:** A testbench is provided to load the RISC-V assembly program and simulate processor behavior.
- **Simulation Scripts:** Include scripts and instructions for compiling and running the simulation using Verilator.
- **Documentation:** This README file along with any additional design documents.

## Future Enhancements

- **Pipelining:** Implementing 5-stage pipelining to improve instruction throughput.
- **Expanded Instruction Set:** Supporting additional instructions beyond the basic RV32I set.
- **Tool Integration:** Incorporating continuous integration (CI) for automated testing and verification.

## Contact

For questions or contributions, please contact:

**Almas Adilgazyuly**  
Email: [almas.adilgazyuly@nu.edu.kz](mailto:almas.adilgazyuly@nu.edu.kz)  
LinkedIn: [linkedin.com/in/almas-adilgazyuly-01293a281](https://www.linkedin.com/in/almas-adilgazyuly-01293a281)
