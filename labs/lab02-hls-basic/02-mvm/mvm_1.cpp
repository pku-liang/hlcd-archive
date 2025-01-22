#include "mvm.h"

void mvm(mat_t mat, vec_t vec, res_t res) {
#pragma HLS INTERFACE mode=m_axi port=mat
#pragma HLS INTERFACE mode=m_axi port=vec
#pragma HLS INTERFACE mode=m_axi port=res

loop_i:
    for(int i = 0; i < N; i++) {
        out_t sum = 0;
loop_j:
        for(int j = 0; j < N; j++) {
            sum += mat[i][j] * vec[j];
        }
        res[i] = sum;
    }
}
