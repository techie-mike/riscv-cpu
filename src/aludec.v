module aludec(input logic OP_5,
              input logic [2:0] FUNCT_3,
              input logic FUNCT_7_5,
              input logic [1:0] ALU_OP,
              output logic [2:0] ALU_CONTROL);
    logic R_TYPE_SUB;
    assign R_TYPE_SUB = FUNCT_7_5 & OP_5; // TRUE for R–type subtract
    always_comb case(ALU_OP)
        2'b00: ALU_CONTROL = 3'b000; // addition
        2'b01: ALU_CONTROL = 3'b001; // subtraction
        default: case(FUNCT_3) // R–type or I–type ALU
                3'b000: if (R_TYPE_SUB)
                    ALU_CONTROL = 3'b001; // sub
                else
                    ALU_CONTROL = 3'b000;   // add, addi
        3'b010:  ALU_CONTROL = 3'b101;      // slt, slti
        3'b110:  ALU_CONTROL = 3'b011;      // or, ori
        3'b111:  ALU_CONTROL = 3'b010;      // and, andi
        default: ALU_CONTROL = 3'bxxx;      // ???
        endcase
    endcase
endmodule
