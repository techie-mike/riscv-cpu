module controller(input logic [6:0] OP,
                  input logic [2:0] FUNCT_3,
                  input logic FUNCT_7_5,
                  input logic ZERO,
                  output logic [1:0] RESULT_SRC,
                  output logic MEM_WRITE, PC_SRC, ALU_SRC, REG_WRITE, JUMP,
                  output logic [1:0] IMM_SRC,
                  output logic [2:0] ALU_CONTROL);
    logic [1:0] ALU_OP;
    logic BRANCH;
    maindec main_decoder(OP, RESULT_SRC, MEM_WRITE, BRANCH,
                ALU_SRC, REG_WRITE, JUMP, IMM_SRC, ALU_OP);
    aludec alu_decoder(OP[5], FUNCT_3, FUNCT_7_5, ALU_OP, ALU_CONTROL);
    assign PC_SRC = BRANCH & ZERO | JUMP;
endmodule
