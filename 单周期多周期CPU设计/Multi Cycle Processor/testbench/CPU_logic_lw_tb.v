`timescale 10ns/1ns
`include "../include/ALU_Ctrl.v"
`include "../include/ALU.v"
`include "../include/CU_logic.v"
`include "../include/Mem.v"
`include "../include/mux2.v"
`include "../include/mux4.v"
`include "../include/Reg.v"
`include "../include/RF.v"
`include "../include/SigExt.v"
`include "../include/CPU_logic.v"

module CPU_logic_lw_tb;
    //指令存储在32'b00000000-32'b00000003的位置
    //数据在32'b00000004--32'b00000007的位置
    //指令内容为 lW $4,(32'h2)($1),
    //即把寄存器1的内容+32'h2作为addr（结果为00000004）
    //取出数赋值给寄存器4
    //对应的机器码为 100011 00001 00100 0000000000000010
    reg clk;
    parameter clk_period = 10;

    initial begin
        $dumpfile("../vcd/CPU_logic_lw_tb.vcd");             
        $dumpvars(0, CPU_logic_lw_tb); 
        $readmemh("../file/lw_RF.txt",cpu.RegFile.RF);//设置RF初值
        $readmemh("../file/lw.txt",cpu.mem.data);//设置存储器初值
        
        clk <= 1 ;
        #0.2 cpu.PC.Q <= 32'h0; //pc赋值
        cpu.CU.S <= 4'b0000;

        #6000 $finish; 
    end

    always #(clk_period/2) clk = ~clk;

    CPU_logic cpu(.clk(clk));

endmodule