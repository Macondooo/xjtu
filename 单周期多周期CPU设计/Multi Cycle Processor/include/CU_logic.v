//组合逻辑的同步控制
//产生控制信号有0.1个时间单位的延时
module CU_logic(PCWr,PCWrCond,IorD,MemRd,MemWr,IRWr,MemtoReg,PCSrc,ALUOp,ALUSrcB,ALUSrcA,RegWr,RegDst,Op,clk);
    input [5:0] Op;
    input clk;

    output PCWr,PCWrCond,IorD,MemRd,MemWr,IRWr,MemtoReg,ALUSrcA,RegWr,RegDst;
    output [1:0] PCSrc;
    output [1:0] ALUOp;
    output [1:0] ALUSrcB;

    reg [3:0] S;//目前的状态
    wire [3:0] NS;//下一个状态
    wire [19:0] P;//中间与门的输出

    //组合逻辑上方的与门
    //P[0]-P[9]是代表0-9的状态
    assign #0.1 P[0] = ~S[3] && ~S[2] && ~S[1] && ~S[0]; //0000
    assign #0.1 P[1] = ~S[3] && ~S[2] && ~S[1] &&  S[0]; //0001
    assign #0.1 P[2] = ~S[3] && ~S[2] &&  S[1] && ~S[0]; //0010
    assign #0.1 P[3] = ~S[3] && ~S[2] &&  S[1] &&  S[0]; //0011
    assign #0.1 P[4] = ~S[3] &&  S[2] && ~S[1] && ~S[0]; //0100
    assign #0.1 P[5] = ~S[3] &&  S[2] && ~S[1] &&  S[0]; //0101
    assign #0.1 P[6] = ~S[3] &&  S[2] &&  S[1] && ~S[0]; //0110
    assign #0.1 P[7] = ~S[3] &&  S[2] &&  S[1] &&  S[0]; //0111  
    assign #0.1 P[8] =  S[3] && ~S[2] && ~S[1] && ~S[0]; //1000
    assign #0.1 P[9] =  S[3] && ~S[2] && ~S[1] &&  S[0]; //1001
    assign #0.1 P[19] = S[3] && ~S[2] &&  S[1] && ~S[0]; //1010
    //下面的是代表什么情况下有分支的情况下
    assign #0.1 P[10] = ~Op[5] && ~Op[4] && ~Op[3] && ~Op[2] &&  Op[1] && ~Op[0] && ~S[3] && ~S[2] && ~S[1] &&  S[0]; //000010 0001 -> j 0001
    assign #0.1 P[11] = ~Op[5] && ~Op[4] && ~Op[3] &&  Op[2] && ~Op[1] && ~Op[0] && ~S[3] && ~S[2] && ~S[1] &&  S[0]; //000100 0001 -> beq 0001
    assign #0.1 P[12] = ~Op[5] && ~Op[4] && ~Op[3] && ~Op[2] && ~Op[1] && ~Op[0] && ~S[3] && ~S[2] && ~S[1] &&  S[0]; //000000 0001 ->R_type 0001
    assign #0.1 P[13] =  Op[5] && ~Op[4] && ~Op[3] && ~Op[2] &&  Op[1] &&  Op[0] && ~S[3] && ~S[2] &&  S[1] && ~S[0]; //100011 0010 ->lw 0010
    assign #0.1 P[14] =  Op[5] && ~Op[4] &&  Op[3] && ~Op[2] &&  Op[1] &&  Op[0] && ~S[3] && ~S[2] && ~S[1] &&  S[0]; //101011 0001 ->sw 0001
    assign #0.1 P[15] =  Op[5] && ~Op[4] && ~Op[3] && ~Op[2] &&  Op[1] &&  Op[0] && ~S[3] && ~S[2] && ~S[1] &&  S[0]; //100011 0001 ->lw 0001
    assign #0.1 P[16] =  Op[5] && ~Op[4] &&  Op[3] && ~Op[2] &&  Op[1] &&  Op[0] && ~S[3] && ~S[2] &&  S[1] && ~S[0]; //101011 0010 ->sw 0010
    //addiu指令新增的
    assign #0.1 P[17] = ~Op[5] && ~Op[4] &&  Op[3] && ~Op[2] && ~Op[1] &&  Op[0] && ~S[3] && ~S[2] && ~S[1] &&  S[0]; //001001 0001 ->addiu 0001
    assign #0.1 P[18] = ~Op[5] && ~Op[4] &&  Op[3] && ~Op[2] && ~Op[1] &&  Op[0] && ~S[3] && ~S[2] &&  S[1] && ~S[0]; //001001 0010 ->addiu 0010

    //组合逻辑下方或门
    assign #0.1 PCWr = P[0] || P[9];
    assign #0.1 PCWrCond = P[8];
    assign #0.1 IorD = P[3] || P[5];
    assign #0.1 MemRd = P[0] || P[3];
    assign #0.1 MemWr =P[5];
    assign #0.1 IRWr = P[0];
    assign #0.1 MemtoReg = P[4];
    assign #0.1 PCSrc = {P[9],P[8]};
    assign #0.1 ALUOp = {P[6],P[8]};
    assign #0.1 ALUSrcB = {P[1]||P[2],P[0]||P[1]};
    assign #0.1 ALUSrcA = P[2] || P[6] || P[8];
    //assign #0.1 RegWr = P[4] || P[7];
    assign #0.1 RegWr = P[4] || P[7] || P[19];
    assign #0.1 RegDst = P[7];
    // assign #0.1 NS[3] = P[10] || P[11];
    // assign #0.1 NS[2] = P[3] || P[6] || P[12] || P[16];
    // assign #0.1 NS[1] = P[6] || P[12] || P[13] || P[14] || P[15];
    // assign #0.1 NS[0] = P[0] || P[6] || P[10] || P[13] || P[16];
    assign #0.1 NS[3] = P[10] || P[11] || P[18];
    assign #0.1 NS[2] = P[3] || P[6] || P[12] || P[16];
    assign #0.1 NS[1] = P[6] || P[12] || P[13] || P[14] || P[15] || P[17] || P[18];
    assign #0.1 NS[0] = P[0] || P[6] || P[10] || P[13] || P[16];


    always @(posedge clk) begin
        #0.1
        S <= NS;
    end

endmodule
