module ALU_CU (ALUOp,func,ALUCtrl);
input [5:0] func;
input [1:0] ALUOp;
output reg [2:0] ALUCtrl;

always @(func or ALUOp) begin
    #0.1
    casex(ALUOp) 
        2'b00: ALUCtrl <= 100;
        2'b01: ALUCtrl <= 110;
        2'b1x: begin
          case (func)
            6'b100000: ALUCtrl <= 100;
            6'b100001: ALUCtrl <= 101;
            6'b100010: ALUCtrl <= 110;
            6'b100100: ALUCtrl <= 000;
            6'b100101: ALUCtrl <= 001;
            6'b101010: ALUCtrl <= 011;
          endcase
        end
    endcase
end
endmodule