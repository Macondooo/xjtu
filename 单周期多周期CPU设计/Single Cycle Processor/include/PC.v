//写数据设置有0.1个时间单位的延迟
//读数据输出类型为reg，一直可读
module PC(clk,data,Q);
input clk;
// input RegOe;
input [31:0] data;
output reg [31:0] Q;

always @(posedge clk ) begin
    #0.1
    Q <=  data;
end

endmodule