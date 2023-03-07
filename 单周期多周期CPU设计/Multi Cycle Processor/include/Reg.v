//用来生成所有的寄存器
//写数据设置有0.1个时间单位的延迟
//读数据输出类型为reg，一直可读
module Reg(clk,data,Wr,Q);
    input clk, Wr;
    input [31:0] data;
    output reg [31:0] Q;

    always @(posedge clk ) begin
        #0.1
        if(Wr)
            Q <=  data;
    end
endmodule