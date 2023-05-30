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
    logic [4:0] INST_RS2 = INSTR[24:20];
    // logic [19:15] INST_RS1;

    logic REG_WRITE /*verilator public*/;

    logic MEM_WR /*verilator public*/;


    ram inst_mem (.CLK(CLK),
                  .A  (PC),
                  .WE (0),
                  .WD (0),
                  .RD (INSTR));

    logic [31:0] IMM_EXT;
    logic [1:0] IMM_SRC /*verilator public*/;
    extend extend (.INSTR(INSTR[31:7]),
                   .IMM_EXT(IMM_EXT),
                   .IMM_SRC(IMM_SRC));

    logic [31:0] RD1, RD2;
    /* verilator lint_off PINMISSING */
    regfile regfile (.CLK(CLK),
                     .A1(INST_RS1),
                     .A2(INST_RS2),
                     .RD1(RD1),
                     .RD2(RD2),
                     .WD3(READ_DATA),
                     .A3(INST_RD),
                     .WE3(REG_WRITE));
    /* verilator lint_off PINMISSING */

    logic [31:0] ALU_RESULT /*verilator public*/;
    alu alu (.SRC_A(RD1), .SRC_B(IMM_EXT), .RES(ALU_RESULT));

    ram data_mem (.CLK(CLK),
                  .A  (ALU_RESULT),
                  .WE (MEM_WR),
                  .WD (RD2),
                  .RD (READ_DATA));

    adder pc_plus4(.SRC_A(PC),
                   .SRC_B(32'd4),
                   .RES(PC_NEW));
endmodule
