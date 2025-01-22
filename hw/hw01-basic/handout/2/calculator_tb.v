module Calculator_tb();
    reg [7:0] a,b;
    reg [1:0] op;
    wire [7:0] y;

    Calculator #(.N(8)) dut(.a(a),.b(b),.op(op),.y(y));

    initial begin
        $dumpvars(0, a, b, op, y);
        a = 8'd123;
        b = 8'd45;
        op = 2'd0;
        #1 $display("A=%d, B=%d, OP=%d, Y=%d", a, b, op, y);
        a = 8'd99;
        b = 8'd72;
        op = 2'd1;
        #1 $display("A=%d, B=%d, OP=%d, Y=%d", a, b, op, y);
        a = 8'd12;
        b = 8'd8;
        op = 2'd2;
        #1 $display("A=%d, B=%d, OP=%d, Y=%d", a, b, op, y);
        a = 8'd240;
        b = 8'd60;
        op = 2'd3;
        #1 $display("A=%d, B=%d, OP=%d, Y=%d", a, b, op, y);
        $finish;
    end
endmodule