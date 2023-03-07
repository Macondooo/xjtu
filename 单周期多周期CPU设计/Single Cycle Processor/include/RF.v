//只有写使能，没有输出使能
//可以同时读两个寄存器，写一个寄存器
//寄存器堆RF有32个32位寄存器
//任何时候都可以读，时钟上跳沿来写数据
module RF(R_Reg1,R_Reg2,W_Reg,W_data,R_data1,R_data2,clk,RegWr);
input [4:0] R_Reg1, R_Reg2, W_Reg;
input [31:0] W_data;
input clk, RegWr;
output [31:0] R_data1, R_data2;

reg [31:0] RF [31:0];//注意第二个是元素个数不是位宽

assign #0.1 R_data1 = RF[R_Reg1];
assign #0.1 R_data2 = RF[R_Reg2];

always @(posedge clk ) begin
    if(RegWr)
        #0.1 RF[W_Reg] <= W_data;
end

endmodule