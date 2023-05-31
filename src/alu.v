`include "const.v"
module alu(input [31:0] SRC_A,
           input [31:0] SRC_B,
           input [2:0] ALU_CONTROL,
           output logic [31:0] RES);
    always_comb begin
        case(ALU_CONTROL)
            `ALU_ADD:
                assign RES = SRC_A + SRC_B;
            `ALU_SUB:
                assign RES = SRC_A - SRC_B;
            `ALU_AND:
                assign RES = SRC_A & SRC_B;
            `ALU_OR:
                assign RES = SRC_A | SRC_B;

            default:
                ;
        endcase
    end
endmodule
