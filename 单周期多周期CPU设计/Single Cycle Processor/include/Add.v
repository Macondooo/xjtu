module Add(Add_a, Add_b, Add_c);
input [31:0] Add_a, Add_b;
output [31:0] Add_c;

assign #0.1 Add_c = Add_a + Add_b;

endmodule