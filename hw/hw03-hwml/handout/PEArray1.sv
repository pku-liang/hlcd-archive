module PEArray1 #(
    parameter W = 8,
    parameter N = 4
)(
    input logic clk, rst, stationaryCtrl,
    input logic [N-1:0][W-1:0] a_in,
    input logic [N-1:0][W-1:0] b_in,
    input logic [N-1:0][W-1:0] c_in,
    output logic [N-1:0][W-1:0] c_out
);
    assign c_out = c_in;
endmodule
