#pragma once

#include "ap_int.h"
#include "hls_vector.h"

using in_t = ap_ufixed<8,5>;
using data_t = ap_ufixed<22,16>;

const int N = 64;
const float eps = 1e-6;
const int VEC = 32;
using vec_t = hls::vector<in_t, VEC>;

void mm(const in_t A[N][N] , const in_t B[N][N], data_t C[N][N]);