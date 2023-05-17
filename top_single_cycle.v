module top_single_cycle (CLK, PC_NEW);
    input wire CLK;
    reg [31:0] PC;
    output wire [31:0] PC_NEW;
    pc PC1 (.CLK (CLK),
            .PC (PC_NEW),
            .PCNext(PC));
    always @(posedge CLK) begin
        PC <= ~PC;
    end

endmodule