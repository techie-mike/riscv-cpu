module mux_3(input [1:0]  CONTROL_SIG,
             input  [31:0] SRC_0, SRC_1, SRC_2,
             output [31:0] RESULT);
    assign RESULT = CONTROL_SIG[1] ? SRC_2 : (CONTROL_SIG[0] ? SRC_1 : SRC_0);
endmodule
