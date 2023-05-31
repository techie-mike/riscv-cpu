module top_single_cycle(input CLK);
    logic [31:0] PC /*verilator public*/;
    logic [31:0] PC_NEW;
    pc pc (.CLK (CLK),
           .PC (PC),
           .PC_NEXT(PC_NEW));

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
    logic RESULT_SRC        /*verilator public*/;
    logic [2:0] ALU_CONTROL /*verilator public*/;
    /* CONTROL SIGNALS */

    logic [31:0] RES_MUX_SRC_ALU;
    logic [31:0] RESULT;


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
                   .RES(PC_NEW));

    mux_2 mux_src_alu(.CONTROL_SIG(ALU_SRC),
                      .SRC_0(RD2),
                      .SRC_1(IMM_EXT),
                      .RESULT(RES_MUX_SRC_ALU));

    mux_2 mux_res_data_mem(.CONTROL_SIG(RESULT_SRC),
                           .SRC_0(ALU_RESULT),
                           .SRC_1(READ_DATA),
                           .RESULT(RESULT));
endmodule
