#include "mm.h"

void mm(const in_t A[N][N], const in_t B[N][N], data_t C[N][N]) {
#pragma HLS interface ap_ctrl_none port = return
#pragma HLS interface m_axi port = A
#pragma HLS interface m_axi port = B
#pragma HLS interface m_axi port = C

  in_t local_A[N][N], local_B[N][N];

  // First, load two matrixes onto the on-chip memory.
  for (int i = 0; i < N; i++)
    for (int k = 0; k < N; k++)
      local_A[i][k] = A[i][k];
  for (int k = 0; k < N; k++)
    for (int j = 0; j < N; j++)
      local_B[k][j] = B[k][j];

  // Do computation, and write the results back to the host.
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      data_t sum = 0;
      for (int k = 0; k < N; k++)
        sum += local_A[i][k] * local_B[k][j];
      C[i][j] = sum;
    }
}
