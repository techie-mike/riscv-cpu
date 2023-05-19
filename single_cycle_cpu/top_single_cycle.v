module top_single_cycle (input CLK);
    // input CLK;
    logic [31:0] PC;
    logic [31:0] PC_NEW;
    pc pc (.CLK (CLK),
           .PC (PC),
           .PCNext(PC_NEW));

    logic [31:0] INSTR /*verilator public*/;

    ram inst_mem (.CLK(CLK),
                  .A  (PC),
                  .WE (0),
                  .WD (0),
                  .RD (INSTR));

endmodule