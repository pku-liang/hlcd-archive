`include "define.svh"

module alu_tb(funct, in_a, in_b, out, gold);

    input op_t funct;
    input int32_t in_a;
    input int32_t in_b;
    output int32_t out;
    output int32_t gold;

    alu alu_0(.*);

    function int32_t alu_gold(op_t funct, int32_t in_a, int32_t in_b);
        case(funct)
            op_zero: alu_gold = `OP_WIDTH'b0;
            op_add:  alu_gold = in_a + in_b;
            op_sub:  alu_gold = in_a - in_b;
            op_shl:  alu_gold = in_a << in_b[4:0];
            op_shr:  alu_gold = in_a >> in_b[4:0];
            op_and:  alu_gold = in_a & in_b;
            op_or:   alu_gold = in_a | in_b;
            op_xor:  alu_gold = in_a ^ in_b;
        endcase
    endfunction

    assign gold = alu_gold(funct, in_a, in_b);

endmodule