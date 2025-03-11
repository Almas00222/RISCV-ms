module instrmemory 
#(
	parameter SIZE = 1024
)
(
    input [31:0] a, wd,     // Address input
    input clk,
    input we,
    output reg [31:0] rd // Instruction output
);
    reg [31:0] RAM [SIZE - 1:0];    // Memory storage (64 instructions max)

    initial begin
        $readmemh("instructions.hex", RAM); // Preload instructions from a file
    end
    
    assign rd = RAM[a[11:2]];
    
    always @(posedge clk) begin
       if (we) RAM[a[11:2]] <= wd;
    end
endmodule
