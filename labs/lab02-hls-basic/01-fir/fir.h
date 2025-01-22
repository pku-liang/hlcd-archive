#pragma once

using coef_t = int;
using data_t = int;
using acc_t  = int;

const int FIR_N = 11;

void fir(acc_t * y, data_t x);