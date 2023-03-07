//这里包含cpu的所有连线
//指令寄存器和数据寄存器也在此连线了

// `include "ALU_CU.v"
// `include "ALU.v"
// `include "CU.v"
// `include "Add.v"
// `include "mux2.v"
// `include "PC.v"
// `include "RAM.v"
// `include "ROM.v"
// `include "RF.v"
// `include "SigExt.v"
module CPU(clk);
input clk;

//控制信号线
wire RegDst, RegWr, ALUSrc, MemRd, MemWr, MemtoReg, Branch, Jump;
wire [1:0] ALUOp;
wire [2:0] ALUCtrl;
//RF上接的线,图中用蓝色标识
wire [31:0] RF_W_data, RF_data1, RF_data2;
wire [4:0] RF_W_Reg;

//指令存储器的输出线，图中用绿色标识
wire [31:0] Inst;
//数据存储器的输出线，图中用橙色标识
wire [31:0] Data;

//PC上连的线,图中用粉色标识
wire [31:0] pc_in, pc_out;
//ALU上连的线，图上用黄色标识
wire [31:0] ALU_c, ALU_b;
wire Zero, O;

//其他较为零散的连线,图中用淡紫色标识
wire [31:0] SigExt_out;//移位器的输出线
wire [31:0] Add_1_c;//add1的输出线
wire [31:0] Add_2_c;//add2的输出线
wire [31:0] mux_4_out;//4号多路选择器的输出线

//控制单元连线
CU MCU (.OP_Code(Inst[31:26]), .RegDst(RegDst), .RegWr(RegWr), .ALUSrc(ALUSrc), .MemRd(MemRd), .MemWr(MemWr), .MemtoReg(MemtoReg), .Branch(Branch), .Jump(Jump), .ALUOp(ALUOp));
ALU_CU alu_cu (.ALUOp(ALUOp), .func(Inst[5:0]), .ALUCtrl(ALUCtrl));

//寄存器和存储器连线
PC pc (.clk(clk), .data(pc_in), .Q(pc_out));
ROM Inst_mem (.R_data(Inst), .Addr(pc_out));
RAM Data_mem (.R_data(Data), .W_data(RF_data2), .Addr(ALU_c), .MemWr(MemWr), .MemRd(MemRd));
RF RegFile (.R_Reg1(Inst[25:21]), .R_Reg2(Inst[20:16]), .W_Reg(RF_W_Reg), .W_data(RF_W_data), .R_data1(RF_data1), .R_data2(RF_data2), .clk(clk), .RegWr(RegWr));

//运算单元连线
Add Add_1 (.Add_a(32'h0004), .Add_b(pc_out), .Add_c(Add_1_c));
Add Add_2 (.Add_a(Add_1_c), .Add_b(SigExt_out<<2), .Add_c(Add_2_c));
ALU alu (.ALU_a(RF_data1), .ALU_b(ALU_b), .ALUCtrl(ALUCtrl), .Zero(Zero), .O(O), .ALU_c(ALU_c));
SigExt sigext (.in(Inst[15:0]), .out(SigExt_out));

//多路选择器连线
mux2 #(4) mux_1 (.in0(Inst[20:16]), .in1(Inst[15:11]), .out(RF_W_Reg), .addr(RegDst));
mux2 mux_2 (.in0(RF_data2), .in1(SigExt_out), .out(ALU_b), .addr(ALUSrc));
mux2 mux_3 (.in0(ALU_c), .in1(Data), .out(RF_W_data), .addr(MemtoReg));
mux2 mux_4 (.in0(Add_1_c), .in1(Add_2_c), .out(mux_4_out), .addr(Zero & Branch));
mux2 mux_5 (.in0(mux_4_out), .in1({Add_1_c[31:28],Inst[25:0],2'b00}), .out(pc_in), .addr(Jump));

endmodule