module maindec(input logic [6:0] OP,
               output logic [1:0] RESULT_SRC,
               output logic MEM_WRITE,
               output logic BRANCH, ALU_SRC,
               output logic REG_WRITE, JUMP,
               output logic [1:0] IMM_SRC,
               output logic [1:0] ALU_OP);
    logic [10:0] CONTROLS;
    assign {REG_WRITE, IMM_SRC, ALU_SRC, MEM_WRITE,
            RESULT_SRC, BRANCH, ALU_OP, JUMP} = CONTROLS;
    always_comb case(OP)
        // |REG_WRITE|IMM_SRC|ALU_SRC|MEM_WRITE|RESULT_SRC|BRANCH|ALU_OP|JUMP|
        7'b0000011: CONTROLS = 11'b1_00_1_0_01_0_00_0; // lw
        7'b0100011: CONTROLS = 11'b0_01_1_1_00_0_00_0; // sw
        7'b0110011: CONTROLS = 11'b1_xx_0_0_00_0_10_0; // R–type
        7'b1100011: CONTROLS = 11'b0_10_0_0_00_1_01_0; // beq
        7'b0010011: CONTROLS = 11'b1_00_1_0_00_0_10_0; // I–type ALU
        7'b1101111: CONTROLS = 11'b1_11_0_0_10_0_00_1; // jal
        default: CONTROLS = 11'bx_xx_x_x_xx_x_xx_x;    // ???
    endcase
endmodule
