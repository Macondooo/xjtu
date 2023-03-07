`timescale 10ns/10ns //第一个是时间单位，第二个是时间精度
`include "../include/Add.v"

module Add_tb;
    wire [31:0] Add_c;
    reg [31:0] a,b;

    initial begin
        $dumpfile("../vcd/Add_tb.vcd");              // 指定记录模拟波形的文件
        $dumpvars(0, Add_tb);          // 指定记录的模块层级                          
        a <= 32'h0011;
        b <= 32'h0001;
        #6000 $finish;  // 6000个单位时间后结束模拟
    end

    always begin
        #20 a = !a;                 // 每20个单位clock取反
    end

    always begin
        #20 b = !b;                 // 每20个单位clock取反
    end

    Add add(.Add_a(a),.Add_b(b),.Add_c(Add_c));

endmodule