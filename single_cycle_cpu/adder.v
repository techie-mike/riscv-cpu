module adder(input [31:0] SRC_A,
             input [31:0] SRC_B,
             output [31:0] RES);
    assign RES = SRC_A + SRC_B;
endmodule