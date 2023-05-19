module pc(input CLK,
          input [31:0] PCNext,
          output reg [31:0] PC);
    initial begin
        PC = 32'h1000;
    end
    always @(posedge CLK)
    begin
        PC <= PC + 0'd1;
    end
endmodule