#include "mm.h"

void mm(const in_t A[N][N], const in_t B[N][N], data_t C[N][N]) {
#pragma HLS interface ap_ctrl_none port=return bundle=mm
#pragma HLS interface m_axi port=A bundle=mm
#pragma HLS interface m_axi port=B bundle=mm
#pragma HLS interface m_axi port=C bundle=mm

  in_t local_B[N][N];

#pragma HLS ARRAY_PARTITION variable=local_B factor=16 dim=2 type=cyclic

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
#pragma HLS PIPELINE II=1 rewind
      local_B[j][i] = B[i][j];
    }

  for (int i = 0; i < N; i++) {
    in_t local_A[N];
    data_t local_C[N];
#pragma HLS ARRAY_PARTITION variable=local_A factor=16 type=cyclic
#pragma HLS DATAFLOW
    for(int j = 0; j < N; j++) {
#pragma HLS PIPELINE II=1 rewind
      local_A[j] = A[i][j];
    }
    for (int j = 0; j < N; j ++) {
      local_C[j] = 0;
      for (int k = 0; k < N; k += 16) {
#pragma HLS PIPELINE II=1 rewind
        for(int kk = 0; kk < 16; kk++) {
#pragma HLS UNROLL
          local_C[j] += local_A[k + kk] * local_B[j][k + kk];
        }
      }
    }
    for(int j = 0; j < N; j++) {
#pragma HLS PIPELINE II=1 rewind
      C[i][j] = local_C[j];
    }
  }
}

