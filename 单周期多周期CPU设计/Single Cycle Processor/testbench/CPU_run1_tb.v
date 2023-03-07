`timescale 10ns/1ns
`include "../include/CPU.v"
`include "../include/ALU_CU.v"
`include "../include/ALU.v"
`include "../include/CU.v"
`include "../include/Add.v"
`include "../include/mux2.v"
`include "../include/PC.v"
`include "../include/RAM.v"
`include "../include/ROM.v"
`include "../include/RF.v"
`include "../include/SigExt.v"

module CPU_run1_tb;
    reg clk;
    parameter clk_period = 10;

    initial begin
        $dumpfile("../vcd/CPU_run1_tb.vcd");             
        $dumpvars(0, CPU_run1_tb); 
        $readmemh("../file/run1.txt",cpu.Inst_mem.mem);//设置指令存储器初值
        $readmemh("../file/run1_RF.txt",cpu.RegFile.RF);//设置RF初值,$0寄存器的值恒为0
        
        clk <= 1 ;
        #0.2 cpu.pc.Q <= 32'h0;

        // #10
        // $display("%h",cpu.RegFile.RF[8]);
        // #10
        // $display("%h",cpu.RegFile.RF[9]);

        #6000 $finish; 
    end

    always #(clk_period/2) clk = ~clk;

    CPU cpu(.clk(clk));

endmodule