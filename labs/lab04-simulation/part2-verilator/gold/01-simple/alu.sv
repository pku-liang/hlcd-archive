`include "define.svh"

module alu(funct, in_a, in_b, out);

    input op_t funct;
    input int32_t in_a;
    input int32_t in_b;
    output int32_t out;

    logic add_carry_in;
    logic add_carry_out;
    int32_t add_op1;
    int32_t add_op2;
    int32_t add_out;

    always_comb begin
        case(funct)
            op_add:  begin
                add_op1 = in_a;
                add_op2 = in_b;
                add_carry_in = 0;
            end
            op_sub:  begin
                add_op1 = in_a;
                add_op2 = ~in_b;
                add_carry_in = 1;
            end
            default: begin
                add_op1 = 0;
                add_op2 = 0;
                add_carry_in = 0;
            end
        endcase
        {add_carry_out, add_out} = {1'd0, add_op1} + {1'd0, add_op2} + {32'd0, add_carry_in};
    end

    int32_t shift_op1;
    logic [4:0] shift_op2;
    int32_t shift_out_tmp;
    int32_t shift_out;

    always_comb begin
        case(funct)
            op_shl: begin
                shift_op1 = in_a;
                shift_op2 = in_b[4:0];
            end
            op_shr: begin
                shift_op1 = {<<{in_a}};
                shift_op2 = in_b[4:0];
            end
            default: begin
                shift_op1 = 0;
                shift_op2 = 0;
            end
        endcase
        shift_out_tmp = shift_op1 << shift_op2;
        case(funct)
            op_shl:  shift_out = shift_out_tmp;
            op_shr:  shift_out = {<<{shift_out_tmp}};
            default: shift_out = shift_out_tmp;
        endcase
    end

    always_comb begin
        case(funct)
            op_add:  out = add_out;
            op_sub:  out = add_out;
            op_shl:  out = shift_out;
            op_shr:  out = shift_out;
            op_and:  out = in_a & in_b;
            op_or:   out = in_a | in_b;
            op_xor:  out = in_a ^ in_b;
            op_zero: out = 0;
        endcase
    end

endmodule