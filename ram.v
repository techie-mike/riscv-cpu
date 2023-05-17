module ram #(parameter SIZE = 7, WORD = 32)
            (input CLK,
             input WE,
             input [SIZE-1:0] A,
             input [WORD-1:0] WD,
             output [WORD-1:0] RD );
    reg [WORD-1:0] mem [2**SIZE-1:0];

    always @(posedge CLK)
    begin
        if (WE)
            mem[A] <= WD;
    end
    assign RD = mem[A];
endmodule