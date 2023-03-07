`timescale 10ns/10ns
`include "../include/RAM.v"
module RAM_tb;
    reg [31:0] Addr;
    reg [31:0] W_data;
    reg MemWr, MemRd;
    wire [31:0] R_data;
    parameter clk_period = 10;
    integer  i;   

    initial begin
        $dumpfile("../vcd/RAM_tb.vcd");              // 指定记录模拟波形的文件
        $dumpvars(0, RAM_tb);          // 指定记录的模块层级
        MemWr <= 0;
        MemRd <= 0;
        Addr <= 32'h0;
        W_data <= 32'h23A0;

        //写入数据
        fork   
            repeat(3) #10 Addr = Addr + 1;
            repeat(10) #7 W_data = W_data + 1;
            repeat(5) #(clk_period/2) MemWr = ~MemWr;
        join

        //把刚才写的数据显示一下
        for (i=0; i<=3; i=i+1) begin
            $display("%h",ram.mem[i]);
        end       


        #30
        Addr <= 32'h0;
        fork
            repeat(3) #10 Addr = Addr + 1;
            repeat(5) #(clk_period/2) MemRd= ~MemRd;
        join
        
        #6000 $finish;  // 6000个单位时间后结束模拟
    end


    //always #(clk_period/2) MemRd = ~MemRd;   

    RAM ram(.Addr(Addr),.W_data(W_data),.MemWr(MemWr),.MemRd(MemRd),.R_data(R_data));


endmodule