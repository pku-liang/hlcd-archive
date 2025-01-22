module PE #(parameter W=8) (
    input logic clk, rst, stationaryCtrl,
    input logic [W-1:0] a_in,
    input logic [W-1:0] b_in,
    input logic [W-1:0] c_in,
    output logic [W-1:0] a_out,
    output logic [W-1:0] b_out,
    output logic [W-1:0] c_out
);
    logic [W-1:0] a, b, c;

    always_ff @(posedge clk) begin
        if (rst) begin
            a <= 0;
            b <= 0;
            c <= 0;
        end else begin
            a <= a_in;
            if (stationaryCtrl)
                b <= b_in;
            c <= c_in + a * b;
        end
    end

    assign a_out = a;
    assign b_out = b;
    assign c_out = c;
endmodule

module PEArray_inner #(
    parameter W = 8,
    parameter N = 4
)(
    input logic clk, rst, stationaryCtrl,
    input logic [N-1:0][W-1:0] a_in,
    input logic [N-1:0][W-1:0] b_in,
    input logic [N-1:0][W-1:0] c_in,
    output logic [N-1:0][W-1:0] c_out
);
    logic [N-1:0][N-1:0][W-1:0] a_ins, b_ins, c_ins;
    logic [N-1:0][N-1:0][W-1:0] a_outs, b_outs, c_outs;

    genvar i, j;
    for (i=0;i<N;i++)
        for (j=0;j<N;j++)
            PE#(.W(W)) pe( .a_in(a_ins[i][j]), .b_in(b_ins[i][j]), .c_in(c_ins[i][j]),
                .a_out(a_outs[i][j]), .b_out(b_outs[i][j]), .c_out(c_outs[i][j]), .*);

    for (j=0;j<N;j++) 
        assign a_ins[0][j] = a_in[j];
    for (i=1;i<N;i++) for (j=0;j<N;j++)
        assign a_ins[i][j] = a_outs[i-1][j];

    for (i=0;i<N;i++)
        assign b_ins[i][0] = b_in[i];
    for (i=0;i<N;i++) for (j=1;j<N;j++)
        assign b_ins[i][j] = b_outs[i][j-1];

    for (i=0;i<N;i++) begin
        assign c_ins[i][0] = c_in[i];
        assign c_out[i] = c_outs[i][N-1];
    end

    for (i=0;i<N;i++) for (j=1;j<N;j++)
        assign c_ins[i][j] = c_outs[i][j-1];
endmodule

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
    PEArray_inner #(.W(W), .N(N)) pe(.*);
endmodule