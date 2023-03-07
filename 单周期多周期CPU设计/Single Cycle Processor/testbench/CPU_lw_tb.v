`timescale 10ns/1ns
`include "../include/CPU.v"
`include "../include/ALU_CU.v"
`include "../include/ALU.v"
`include "../include/CU.v"
`include "../include/Add.v"
`include "../include/mux2.v"
`include "../include/mux3.v"
`include "../include/PC.v"
`include "../include/RAM.v"
`include "../include/ROM.v"
`include "../include/RF.v"
`include "../include/SigExt.v"

module CPU_lw_tb;
    reg clk;
    parameter clk_period = 10;

    initial begin
        $dumpfile("../vcd/CPU_lw_tb.vcd");             
        $dumpvars(0, CPU_lw_tb); 
        $readmemh("../file/RF.txt",cpu.RegFile.RF);//设置RF初值
        $readmemh("../file/temp.txt",cpu.Data_mem.mem);//设置数据存储器初值
        $readmemb("../file/lw.txt",cpu.Inst_mem.mem);//设置指令存储器初值
        
        clk <= 1 ;
        #0.2 cpu.pc.Q <= 32'h0;

        #6000 $finish; 
    end

    always #(clk_period/2) clk = ~clk;

    CPU cpu(.clk(clk));

endmodule