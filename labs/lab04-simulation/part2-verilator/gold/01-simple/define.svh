`ifndef _DEFINE_H_
`define _DEFINE_H_

`define OP_WIDTH 32

typedef enum logic [2:0] {
    op_zero = 0,
    op_add = 1,
    op_sub = 2,
    op_shl = 3,
    op_shr = 4,
    op_and = 5,
    op_or  = 6,
    op_xor = 7
} op_t;

typedef logic [`OP_WIDTH-1:0] int32_t;

`endif