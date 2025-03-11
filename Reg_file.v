module Reg_file(
	input 		  clk,
	input 		  we3,
	input 	   [4:0]  a1,
	input 	   [4:0]  a2,
	input 	   [4:0]  a3,
	input 	   [31:0] wd3,
	output reg [31:0] rd1,
	output reg [31:0] rd2
);
	
	 (* verilator public *)
	reg [31:0] rf [31:0];

	always @(posedge clk) begin
		if (we3 == 1) rf[a3] <= wd3;
	assign rd1 = a1 == 0 ? 0 : rf[a1];
	assign rd2 = a2 == 0 ? 0 : rf[a2];
	end
endmodule
		
