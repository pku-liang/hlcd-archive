#include "fir.h"

void fir(acc_t *y, data_t x) {
    coef_t coef[FIR_N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0, 53};
    static data_t shift_reg[FIR_N] = {};
    acc_t acc = 0;
fir_loop:
    for(int i = FIR_N - 1; i >= 0; i--) {
        if(i == 0) {
            acc += x * coef[0];
            shift_reg[0] = x;
        } else {
            shift_reg[i] = shift_reg[i - 1];
            acc += shift_reg[i] * coef[i];
        }
    }
    *y = acc;
}