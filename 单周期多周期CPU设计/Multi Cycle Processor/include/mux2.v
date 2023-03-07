//parameter指定输入的位数
//多路选择器产生选通规定有0.1个时间单位的延时
module mux2(in0,in1,out,addr);
parameter width = 31;
input [width:0] in0, in1;
input addr;
output [width:0] out;

assign #0.1 out = addr? in1 : in0;

endmodule