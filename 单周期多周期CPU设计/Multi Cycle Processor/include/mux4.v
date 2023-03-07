//多路选择器产生选通规定有0.1个时间单位的延时
module mux4(in0,in1,in2,in3,out,addr);
parameter width = 31;
input [width:0] in0, in1, in2, in3;
input [1:0] addr;
output reg [width:0] out;

always @(addr or in0 or in1 or in2 or in3) begin
    #0.1
    case(addr)
    2'b00: out <= in0;
    2'b01: out <= in1;
    2'b10: out <= in2;
    2'b11: out <= in3;
    endcase
end
endmodule