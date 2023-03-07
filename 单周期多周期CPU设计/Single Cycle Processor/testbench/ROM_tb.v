`timescale 10ns/10ns
`include "../include/ROM.v"
module ROM_tb;
    wire [31:0] Addr;
    wire [31:0] R_data;

    initial begin
        $dumpfile("../vcd/ROM_tb.vcd");              // 指定记录模拟波形的文件
        $dumpvars(0, ROM_tb);          // 指定记录的模块层级             
        $readmemh("../file/temp.txt",rom.mem,0,2);
        //$display("%h",rom.mem[32'h00000001]);

        #6000 $finish;  // 6000个单位时间后结束模拟
    end

    assign #10 Addr = 32'h1; //assign是连续赋值，始终处于活动状态

    ROM rom(.R_data(R_data),.Addr(Addr));

endmodule