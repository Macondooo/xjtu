module SigExt(in,out);
input wire signed [15:0] in;
output wire signed [31:0] out;

assign #0.1 out = {{16{in[15]}}, in};


endmodule
