module top_single_cycle(input CLK);
    logic [31:0] PC;
    logic [31:0] PC_NEW;
    pc pc (.CLK (CLK),
           .PC (PC),
           .PC_NEXT(PC_NEW));

    logic [31:0] INSTR /*verilator public*/;
    logic [31:0] READ_DATA;

    logic [4:0] INST_RD = INSTR[11:7];
    logic [4:0] INST_RS1 = INSTR[19:15];
    // logic [19:15] INST_RS1;

    logic REG_WRITE = 1;


    ram inst_mem (.CLK(CLK),
                  .A  (PC),
                  .WE (0),
                  .WD (0),
                  .RD (INSTR));

    logic [31:0] IMM_EXT;
    extend extend (.INSTR(INSTR[31:7]),
                   .IMM_EXT(IMM_EXT),
                   .IMM_SRC(0));

    logic [31:0] RD1;
    /* verilator lint_off PINMISSING */
    regfile regfile (.CLK(CLK),
                     .A1(INST_RS1),
                     .RD1(RD1),
                     .WD3(READ_DATA),
                     .A3(INST_RD),
                     .WE3(REG_WRITE));
    /* verilator lint_off PINMISSING */

    logic [31:0] ALU_RESULT;
    alu alu (.SRC_A(RD1), .SRC_B(IMM_EXT), .RES(ALU_RESULT));

    ram data_mem (.CLK(CLK),
                  .A  (ALU_RESULT),
                  .WE (0),
                  .WD (0),
                  .RD (READ_DATA));

    adder pc_plus4(.SRC_A(PC),
                   .SRC_B(32'd4),
                   .RES(PC_NEW));
endmodule