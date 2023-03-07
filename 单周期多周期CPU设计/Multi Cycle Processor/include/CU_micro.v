//`include "mux4.v"
//有0.1个时间单位的延时
module CU_micro(PCWr,PCWrCond,IorD,MemRd,MemWr,IRWr,MemtoReg,PCSrc,ALUOp,ALUSrcB,ALUSrcA,RegWr,RegDst,Op,clk);
    input [5:0] Op;
    input clk;
    output PCWr,PCWrCond,IorD,MemRd,MemWr,IRWr,MemtoReg,ALUSrcA,RegWr,RegDst;
    output [1:0] PCSrc;
    output [1:0] ALUOp;
    output [1:0] ALUSrcB;

    //内部的连线和部件
    wire [3:0] mux_out;//多路选择器输出
    wire [1:0] mux_addr;//选择mux的地址
    wire [2:0] decode;//译码器的输入
    wire [3:0] addr1; //转移使用组合逻辑输出线
    wire [3:0] addr2;
    wire j,beq,R_type,lw,sw,addiu;//组合逻辑的中间变量，判断输入的Opcode是什么
    //00为转移到0号单元，01为组合逻辑1转移，10为组合逻辑2转移，11为顺序执行
    reg [3:0] CMAR;//用来存储当前地址的位置，clk来的时候更新
    reg [14:0] rom [10:0]; //用来存放微指令
   
   
    initial begin
        $readmemb("../file/rom.txt",rom,0,10);
    end

    //rom输出
    assign #0.1 {PCWr,PCWrCond,IorD,MemRd,decode,ALUOp,ALUSrcB,ALUSrcA,RegWr,mux_addr} = rom[CMAR];
    //译码器
    assign #0.1 RegDst = (decode==3'b001)? 1 : 0;
    assign #0.1 PCSrc[0] = (decode==3'b010)? 1 : 0;
    assign #0.1 PCSrc[1] = (decode==3'b011)? 1 : 0;
    assign #0.1 MemtoReg = (decode==3'b100)? 1 : 0;
    assign #0.1 MemWr = (decode==3'b101)? 1 : 0;
    assign #0.1 IRWr = (decode==3'b110)? 1 : 0;

    //先计算组合逻辑中间变量
    assign #0.1 R_type = ~Op[5] && ~Op[4] && ~Op[3] && ~Op[2] && ~Op[1] && ~Op[0];
    assign #0.1 j =      ~Op[5] && ~Op[4] && ~Op[3] && ~Op[2] &&  Op[1] && ~Op[0];
    assign #0.1 beq =    ~Op[5] && ~Op[4] && ~Op[3] &&  Op[2] && ~Op[1] && ~Op[0];
    assign #0.1 lw =      Op[5] && ~Op[4] && ~Op[3] && ~Op[2] &&  Op[1] &&  Op[0];
    assign #0.1 sw =      Op[5] && ~Op[4] &&  Op[3] && ~Op[2] &&  Op[1] &&  Op[0];
    assign #0.1 addiu  = ~Op[5] && ~Op[4] &&  Op[3] && ~Op[2] && ~Op[1] &&  Op[0];
    //addr1组合逻辑
    assign #0.1 addr1[3] = j || beq;
    assign #0.1 addr1[2] = R_type;
    assign #0.1 addr1[1] = R_type || lw || sw || addiu;
    assign #0.1 addr1[0] = j;
    //addr2组合逻辑
    assign #0.1 addr2[3] = addiu;
    assign #0.1 addr2[2] = sw;
    assign #0.1 addr2[1] = lw || addiu;
    assign #0.1 addr2[0] = lw || sw;

    //多路选择器输出mux_addr
    mux4 #(3) MUX(.in0(4'b0),.in1(addr1),.in2(addr2),.in3(CMAR+4'b1),.out(mux_out),.addr(mux_addr));    
    
    always @(posedge clk) begin //时钟信号到达更新地址
        CMAR <= mux_out;
    end

endmodule