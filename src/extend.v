module extend(input  logic [31:7] INSTR,
              input  logic [1:0]  IMM_SRC,
              output logic [31:0] IMM_EXT);
    always_comb
        case(IMM_SRC)
                        // I type
                2'b00: IMM_EXT = {{20{INSTR[31]}}, INSTR[31:20]};

                        // S−type (stores)
                2'b01: IMM_EXT = {{20{INSTR[31]}}, INSTR[31:25], INSTR[11:7]};
                        // B−type (branches)
                2'b10: IMM_EXT = {{20{INSTR[31]}}, INSTR[7], INSTR[30:25], INSTR[11:8], 1'b0};
                        // J−type (jal)
                2'b11: IMM_EXT = {{12{INSTR[31]}}, INSTR[19:12], INSTR[20], INSTR[30:21], 1'b0};
                default: IMM_EXT = 32'bx; // undefined
        endcase
endmodule