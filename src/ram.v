module ram #(parameter SIZE = 14, WORD = 32) // 2 ^ 13 = 16384 byte
            (input CLK,
             input WE,
             input [WORD-1:0] A,
             input [WORD-1:0] WD,
             output [WORD-1:0] RD);
    reg [WORD-1:0] mem [2**SIZE-1:0] /*verilator public*/;

    always @(posedge CLK)
    begin
        if (WE)
            mem[A[15:2]] <= WD;
    end
    assign RD = mem[A[15:2]];
endmodule
