module top_single_cycle(input CLK);
    logic [31:0] PC /*verilator public*/;
    logic [31:0] PC_CALC /*verilator public*/;
    logic [31:0] PC_NEXT /*verilator public*/;
    logic [31:0] PC_JUMP /*verilator public*/;

    pc pc (.CLK (CLK),
           .PC (PC),
           .PC_NEXT(PC_NEXT));

    logic [31:0] INSTR /*verilator public*/;
    logic [31:0] READ_DATA;

    logic [4:0] INST_RD = INSTR[11:7];
    logic [4:0] INST_RS1 = INSTR[19:15];
    logic [4:0] INST_RS2 = INSTR[24:20];

    /* CONTROL SIGNALS */
    logic REG_WRITE         /*verilator public*/;
    logic MEM_WR            /*verilator public*/;
    logic [31:0] ALU_RESULT /*verilator public*/;
    logic ALU_SRC           /*verilator public*/;
    logic [1:0] RESULT_SRC  /*verilator public*/;
    logic [2:0] ALU_CONTROL /*verilator public*/;
    logic ZERO_FLAG         /*verilator public*/;
    logic PC_SRC            /*verilator public*/;

    /* CONTROL SIGNALS */

    logic [31:0] RES_MUX_SRC_ALU;
    logic [31:0] RESULT;

    assign ZERO_FLAG = (ALU_RESULT == 0);

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
    regfile regfile (.CLK(CLK),
                     .A1(INST_RS1),
                     .A2(INST_RS2),
                     .RD1(RD1),
                     .RD2(RD2),
                     .WD3(RESULT),
                     .A3(INST_RD),
                     .WE3(REG_WRITE));

    alu alu (.SRC_A(RD1), .SRC_B(RES_MUX_SRC_ALU), .RES(ALU_RESULT), .ALU_CONTROL(ALU_CONTROL));

    ram data_mem (.CLK(CLK),
                  .A  (ALU_RESULT),
                  .WE (MEM_WR),
                  .WD (RD2),
                  .RD (READ_DATA));

    adder pc_plus4(.SRC_A(PC),
                   .SRC_B(32'd4),
                   .RES(PC_CALC));

    mux_2 mux_src_alu(.CONTROL_SIG(ALU_SRC),
                      .SRC_0(RD2),
                      .SRC_1(IMM_EXT),
                      .RESULT(RES_MUX_SRC_ALU));

    mux_3 mux_res_data_mem(.CONTROL_SIG(RESULT_SRC),
                           .SRC_0(ALU_RESULT),
                           .SRC_1(READ_DATA),
                           .SRC_2(PC_CALC),
                           .RESULT(RESULT));

    adder pc_target(.SRC_A(PC),
                   .SRC_B(IMM_EXT),
                   .RES(PC_JUMP));

    mux_2 pc_src(.CONTROL_SIG(PC_SRC),
                 .SRC_0(PC_CALC),
                 .SRC_1(PC_JUMP),
                 .RESULT(PC_NEXT));

    logic JUMP;
    controller controller(.OP(INSTR[6:0]),
                          .FUNCT_3(INSTR[14:12]),
                          .FUNCT_7_5(INSTR[30]),
                          .ZERO(ZERO_FLAG),
                          .RESULT_SRC(RESULT_SRC),
                          .MEM_WRITE(MEM_WR),
                          .PC_SRC(PC_SRC),
                          .ALU_SRC(ALU_SRC),
                          .REG_WRITE(REG_WRITE),
                          .JUMP(JUMP),
                          .IMM_SRC(IMM_SRC),
                          .ALU_CONTROL(ALU_CONTROL));

endmodule
