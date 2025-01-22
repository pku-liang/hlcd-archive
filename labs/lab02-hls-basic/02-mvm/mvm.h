#pragma once

#include "ap_fixed.h"

using in_t = ap_fixed<8, 5>;
using out_t = ap_fixed<22, 16>;
const int N = 64;

using mat_t = in_t[N][N];
using vec_t = in_t[N];
using res_t = out_t[N];

void mvm(mat_t mat, vec_t vec, res_t res);