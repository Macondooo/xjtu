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

module CPU_logic_run1_tb;
    //.txt
    // main:
    // 	addiu $t0,$0,1
    // 	addiu $t1,$0,3
    // 	addiu $t3,$0,4
    // 	add $t2,$t0,$t1
    // 	beq $t2,$t3,next
    // 	sub $t0,$t1,$t0
    // next:	
    // 	sub $t0,$t0,$t1	
    //指令第一条存在32'h00000000的位置
    reg clk;
    parameter clk_period = 10;

    initial begin
        $dumpfile("../vcd/CPU_logic_run1_tb.vcd");             
        $dumpvars(0, CPU_logic_run1_tb); 
        $readmemh("../file/run1.txt",cpu.mem.data);//设置存储器初值
        $readmemh("../file/run1_RF.txt",cpu.RegFile.RF);//设置RF初值,$0寄存器的值恒为0
        
        clk <= 1 ;
        #0.2 cpu.PC.Q <= 32'h0; //pc赋值
        cpu.CU.S <= 4'b0000;

        // #10
        // $display("%h",cpu.RegFile.RF[8]);
        // #10
        // $display("%h",cpu.RegFile.RF[9]);

        #6000 $finish; 
    end

    always #(clk_period/2) clk = ~clk;

    CPU_logic cpu(.clk(clk));

endmodule