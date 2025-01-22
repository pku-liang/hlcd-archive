#include "mvm.h"

void mvm(mat_t mat, vec_t vec, res_t res) {
#pragma HLS INTERFACE mode=m_axi port=mat bundle=gmem1
#pragma HLS INTERFACE mode=m_axi port=vec bundle=gmem2
#pragma HLS INTERFACE mode=m_axi port=res bundle=gmem1

#pragma HLS array_partition variable=vec       type=cyclic factor=16
#pragma HLS array_partition variable=mat dim=2 type=cyclic factor=16

loop_i:
    for(int i = 0; i < N; i++) {
        out_t sum = 0;
loop_j:
        for(int j = 0; j < N; j += 16) {
            #pragma HLS PIPELINE II=1 rewind
            for(int jj = 0; jj < 16; jj++) {
                #pragma HLS UNROLL
                sum += mat[i][j + jj] * vec[j + jj];
            }
        }
        res[i] = sum;
    }
}