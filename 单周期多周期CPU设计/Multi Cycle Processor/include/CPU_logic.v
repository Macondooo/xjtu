// `include "ALU_Ctrl.v"
// `include "ALU.v"
// `include "CU_logic.v"
// `include "Mem.v"
// `include "mux2.v"
// `include "mux4.v"
// `include "Reg.v"
// `include "RF.v"
// `include "SigExt.v"

module CPU_logic(clk);
input clk;

//控制信号线
wire PCWr,PCWrCond,IorD,MemRd,MemWr,IRWr,MemtoReg,ALUSrcA,RegWr,RegDst;
wire [1:0] PCSrc;
wire [1:0] ALUOp;
wire [1:0] ALUSrcB;
wire [2:0] ALUCtrl;
wire Zero,O;

wire [31:0] Inst;//指令线，紫色的
wire [31:0] PC_out;//PC线，蓝色的
wire [31:0] PC_in;
wire [31:0] ALU_a;//ALU线和ALU_Out的输出线，绿色的
wire [31:0] ALU_b;
wire [31:0] ALU_c;
wire [31:0] ALU_Out_out;
wire [31:0] mem_addr;//存储器mem相关连线
wire [31:0] mem_rd;
wire [31:0] mem_wd;
wire [31:0] SigExt_out;//移位器输出线,黄色
wire [31:0] MDR_out;//MDR输出线，黄色
wire [31:0] A_out;//A的输出线，黄色
wire [31:0] RF_data1;//RF有关的线，墨绿色
wire [31:0] RF_data2;
wire [31:0] RF_W_data;
wire [4:0] RF_W_Reg;

//控制器
CU_logic CU(.PCWr(PCWr), .PCWrCond(PCWrCond), .IorD(IorD), .MemRd(MemRd), .MemWr(MemWr), .IRWr(IRWr), .MemtoReg(MemtoReg), .PCSrc(PCSrc), .ALUOp(ALUOp), .ALUSrcB(ALUSrcB), .ALUSrcA(ALUSrcA), .RegWr(RegWr), .RegDst(RegDst), .Op(Inst[31:26]), .clk(clk));
ALU_Ctrl alu_ctrl(.ALUOp(ALUOp), .func(Inst[5:0]), .ALUCtrl(ALUCtrl));

//运算器
ALU alu(.ALU_a(ALU_a), .ALU_b(ALU_b), .ALUCtrl(ALUCtrl), .Zero(Zero), .O(O), .ALU_c(ALU_c));

//存储器
Mem mem(.R_data(mem_rd), .W_data(mem_wd), .Addr(mem_addr), .MemWr(MemWr), .MemRd(MemRd));

//寄存器堆
RF RegFile(.R_Reg1(Inst[25:21]), .R_Reg2(Inst[20:16]), .W_Reg(RF_W_Reg), .W_data(RF_W_data), .R_data1(RF_data1), .R_data2(RF_data2), .clk(clk), .RegWr(RegWr));

//寄存器
Reg PC(.clk(clk), .data(PC_in), .Wr(PCWr||(PCWrCond&&Zero)), .Q(PC_out));
Reg IR(.clk(clk), .data(mem_rd), .Wr(IRWr), .Q(Inst));
Reg MDR(.clk(clk), .data(mem_rd), .Wr(1'b1), .Q(MDR_out));
Reg A(.clk(clk), .data(RF_data1), .Wr(1'b1), .Q(A_out));
Reg B(.clk(clk), .data(RF_data2), .Wr(1'b1), .Q(mem_wd));
Reg ALU_Out(.clk(clk), .data(ALU_c), .Wr(1'b1), .Q(ALU_Out_out));

//多路选择器
mux2 mux2_1(.in0(PC_out), .in1(ALU_Out_out), .out(mem_addr), .addr(IorD));
mux2 mux2_2(.in0(ALU_Out_out), .in1(MDR_out), .out(RF_W_data), .addr(MemtoReg));
mux2 #(4) mux2_3(.in0(Inst[20:16]), .in1(Inst[15:11]), .out(RF_W_Reg), .addr(RegDst));
mux2 mux2_4(.in0(PC_out), .in1(A_out), .out(ALU_a), .addr(ALUSrcA));
mux4 mux4_1(.in0(mem_wd), .in1(32'h00000004), .in2(SigExt_out), .in3(SigExt_out<<2), .out(ALU_b), .addr(ALUSrcB));
mux4 mux4_2(.in0(ALU_c), .in1(ALU_Out_out), .in2({PC_out[31:28],Inst[25:0],2'b00}), .in3(32'hzzzzzzzz), .out(PC_in), .addr(PCSrc));

//移位器
SigExt sigext (.in(Inst[15:0]), .out(SigExt_out));

endmodule