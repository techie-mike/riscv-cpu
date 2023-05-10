module ram #(parameter SIZE = 7, WORD = 32)
            (input clk,
             input we,
             input [SIZE-1:0] addr,
             input [WORD-1:0] i_data,
             output [WORD-1:0] o_data );
    reg [WORD-1:0] mem [2**SIZE-1:0];

    always @(posedge clk)
    begin
        if (we)
            mem[addr] <= i_data;
    end
    assign o_data = mem[addr];
endmodule