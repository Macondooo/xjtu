//有写使能和输出使能，可以同时读一个和写一个
//输出和写入有1个时间单位的延时
module Mem(R_data,W_data,Addr,MemWr,MemRd);
input [31:0] Addr,W_data;
input MemWr, MemRd;
output [31:0] R_data;

reg [7:0] data [16'hffff:0]; //size:2^16 x Byte =64KB

//输出的状态
assign #1 R_data = MemRd? {data[Addr+3],data[Addr+2],data[Addr+1],data[Addr]} : 32'hzzzz; //连续赋值assign始终处于活动状态

//写入的状态
always @(posedge MemWr ) begin
    #1 
    data[Addr+3] <= W_data[31:24];
    data[Addr+2] <= W_data[23:16];
    data[Addr+1] <= W_data[15:8];
    data[Addr] <= W_data[7:0];
end

endmodule