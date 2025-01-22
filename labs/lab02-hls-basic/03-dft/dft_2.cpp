#include "dft.h"
#include "coefficients1024.h"

void dft(DTYPE real_in[N], DTYPE imag_in[N],DTYPE real_out[N],DTYPE imag_out[N]) {
#pragma HLS interface m_axi port=real_in
#pragma HLS interface m_axi port=imag_in
#pragma HLS interface m_axi port=real_out
#pragma HLS interface m_axi port=imag_out
	DTYPE real_buf[N], imag_buf[N];
	DTYPE real_ans[N], imag_ans[N];
	memcpy(real_buf, real_in, N * sizeof(DTYPE));
	memcpy(imag_buf, imag_in, N * sizeof(DTYPE));
    memset(real_ans, 0, sizeof(real_ans));
    memset(imag_ans, 0, sizeof(imag_ans));
	for(int j = 0; j < N; j++) {
		int exp = 0;
		for(int i = 0; i < N; i++) {
			float real = cos_coefficients_table[exp];
			float imag = sin_coefficients_table[exp];
			float real_x = real_buf[j], imag_x = imag_buf[j];
			real_ans[i] += real * real_x - imag * imag_x;
			imag_ans[i] += real * imag_x + imag * real_x;
			exp += j;
			if (exp >= N) exp -= N;
		}
	}
	memcpy(real_out, real_ans, N * sizeof(DTYPE));
	memcpy(imag_out, imag_ans, N * sizeof(DTYPE));
}
