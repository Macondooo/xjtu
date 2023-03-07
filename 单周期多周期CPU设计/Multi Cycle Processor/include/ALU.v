//ALU计算有0.1时间单位的延时
module ALU(ALU_a,ALU_b,ALUCtrl,Zero,O,ALU_c);
input [31:0] ALU_a,ALU_b;
input [2:0] ALUCtrl;
output reg [31:0] ALU_c;
output reg Zero;
output reg O;

parameter ALU_pause=0,
          AND=3'b000,//
          OR=3'b001,//
          ADD=3'b100,//
          SUB=3'b110,//
          NOR=3'b011,//
          ADDU=3'b101;

always @(ALU_a or ALU_b or ALUCtrl) begin
    #0.1
    case(ALUCtrl)
        AND: ALU_c <= ALU_a & ALU_b;
        OR: ALU_c <= ALU_a | ALU_b;
        ADD: {O,ALU_c} <= ALU_a + ALU_b;
        SUB: {O,ALU_c} <= ALU_a - ALU_b;
        NOR: ALU_c <= ~ (ALU_a | ALU_b);
        ADDU: ALU_c <= ALU_a + ALU_b;
    endcase
end

always @(ALU_c) begin
    if (ALU_c == 0)
        Zero <= 1;
    else 
        Zero <= 0;
end

endmodule