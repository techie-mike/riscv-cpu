module mux_2 (input  CONTROL_SIG,
              input  [31:0] SRC_0,
              input  [31:0] SRC_1,
              output [31:0] RESULT);
    assign RESULT = CONTROL_SIG ? SRC_1 : SRC_0;
endmodule
