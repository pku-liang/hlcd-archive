module PE #(parameter W=8) (
    input logic clk, rst, stationaryCtrl,
    input logic [W-1:0] a_in,
    input logic [W-1:0] b_in,
    output logic [W-1:0] a_out,
    output logic [W-1:0] c_out
);
    logic [W-1:0] a;

    always_ff @(posedge clk) begin
        if (rst) begin
            a <= 0;
        end else begin
            if (stationaryCtrl)
                a <= a_in;
        end
    end

    assign a_out = a;
    assign c_out = a * b_in;
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
    logic [N-1:0][N-1:0][W-1:0] a_ins;
    logic [N-1:0][N-1:0][W-1:0] a_outs, c_outs;
    logic [N-1:0][W-1:0] c;

    genvar i, j;
    for (i=0;i<N;i++)
        for (j=0;j<N;j++)
            PE#(.W(W)) pe( .a_in(a_ins[i][j]), .b_in(b_in[j]),
                .a_out(a_outs[i][j]), .c_out(c_outs[i][j]), .*);

    for (j=0;j<N;j++) 
        assign a_ins[0][j] = a_in[j];
    for (i=1;i<N;i++) for (j=0;j<N;j++)
        assign a_ins[i][j] = a_outs[i-1][j];

    for (i=0;i<N;i++) begin
        logic [2:0][W-1:0] layer;
        always_ff @(posedge clk) begin
            if (rst) begin
                layer[0] <= 0;
                layer[1] <= 0;
                layer[2] <= 0;
                c[i] <= 0;
            end else begin
                layer[0] <= layer[1] + layer[2];
                layer[1] <= c_outs[i][0] + c_outs[i][1];
                layer[2] <= c_outs[i][2] + c_outs[i][3];
                c[i] <= c_in[i] + layer[0];
            end
        end
        assign c_out[i] = c[i];
    end
endmodule

module PEArray2 #(
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