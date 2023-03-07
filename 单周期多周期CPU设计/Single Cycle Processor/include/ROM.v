//只读寄存器
module ROM(R_data,Addr);
input [31:0] Addr;
output [31:0] R_data;

reg [7:0] mem [16'hffff:0]; //size:2^16 x Byte =64KB

//输出
assign #1 R_data = {mem[Addr+3],mem[Addr+2],mem[Addr+1],mem[Addr]};//小端模式，高字节高地址

endmodule