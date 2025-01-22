/*
/*
This is traditional 2-radix DIT FFT algorithm implementation.
INPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal

OUTPUT:
	Out_R, Out_I[]: Real and Imag parts of Complex signal
*/

#include "fft.h"
#include "hls_stream.h"
#include <iostream>
void bit_reverse(DTYPE X_R[N], DTYPE X_I[N], DTYPE OUT_R[N], DTYPE OUT_I[N]);
void fft_stage(int stage, DTYPE X_R[N], DTYPE X_I[N], DTYPE OUT_R[N], DTYPE OUT_I[N]);
void store_results(DTYPE X[N], DTYPE OUT[N]);

void fft(DTYPE X_R[N], DTYPE X_I[N], DTYPE OUT_R[N], DTYPE OUT_I[N]) {
#pragma HLS interface m_axi port=X_R   	bundle=gmem1
#pragma HLS interface m_axi port=X_I	bundle=gmem1
#pragma HLS interface m_axi port=OUT_R	bundle=gmem1
#pragma HLS interface m_axi port=OUT_I	bundle=gmem2

#pragma HLS dataflow

	DTYPE stage_R[M+1][N], stage_I[M+1][N];
#pragma HLS array_partition variable=stage_R dim=1 type=complete
#pragma HLS array_partition variable=stage_I dim=1 type=complete

	bit_reverse(X_R, X_I, stage_R[0], stage_I[0]);

	for (int stage=0; stage<M; stage++) {
#pragma HLS unroll
		fft_stage(stage+1, stage_R[stage], stage_I[stage], stage_R[stage+1], stage_I[stage+1]);
	}

	store_results(stage_R[M], OUT_R);
	store_results(stage_I[M], OUT_I);
}

void bit_reverse(DTYPE X_R[N], DTYPE X_I[N], DTYPE OUT_R[N], DTYPE OUT_I[N]) {
	for (int i=0; i<N; i++) {
		int rev = 0;
		for (int k=0; k<M; k++) {
			rev = (rev << 1) | ((i >> k) & 1);
		}
		OUT_R[rev] = X_R[i];
		OUT_I[rev] = X_I[i];
	}
}

void fft_stage(int stage, DTYPE X_R[N], DTYPE X_I[N], DTYPE OUT_R[N], DTYPE OUT_I[N]) {
//	assert(stage <= M);
	int m = 1 << stage, mh = 1 << (stage-1);
	int offset = N >> stage;
	float w_r, w_i, u_r, u_i, a_r, a_i, t_r, t_i;
	for (int k = 0; k < N; k += m) {
		int exp = 0;
		for (int j = 0; j < mh; j++) {
			w_r = W_real[exp], w_i = W_imag[exp];
			u_r = X_R[k+j], u_i = X_I[k+j];
			a_r = X_R[k+j+mh], a_i = X_I[k+j+mh];
			t_r = (w_r * a_r - w_i * a_i);
			t_i = (w_r * a_i + w_i * a_r);
			OUT_R[k+j] = u_r + t_r;
			OUT_I[k+j] = u_i + t_i;
			OUT_R[k+j+mh] = u_r - t_r;
			OUT_I[k+j+mh] = u_i - t_i;
			exp += offset;
		}
	}
}

void store_results(DTYPE X[N], DTYPE OUT[N]) {
	memcpy(OUT, X, N * sizeof(DTYPE));
}
