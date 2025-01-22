#include "fft.h"
#include "hls_stream.h"
#include <iostream>


typedef hls::stream<DTYPE> stream_t;
void bit_reverse(DTYPE X_R[N], DTYPE X_I[N], stream_t &, stream_t &);

template<int stage>
void fft_stage_small(stream_t &, stream_t &, stream_t &, stream_t &);
template<int stage>
void fft_stage_large(stream_t &, stream_t &, stream_t &, stream_t &);

void store_results(stream_t &, DTYPE[N]);

template<int m, int scale>
struct FOR;

template<int m>
struct FOR<m, 2> {
	constexpr static int scale = (m>=3) ? 2 : ((m>0)? 1 : 0);
	static void unroll(stream_t sr[M+1], stream_t si[M+1]) {
		FOR<m-1, scale>::unroll(sr, si);
		fft_stage_large<m+1>(sr[m], si[m], sr[m+1], si[m+1]);
	}
};
template<int m>
struct FOR<m, 1> {
	constexpr static int scale = (m>0)? 1 : 0;
	static void unroll(stream_t sr[M+1], stream_t si[M+1]) {
		FOR<m-1, scale>::unroll(sr, si);
		fft_stage_small<m+1>(sr[m], si[m], sr[m+1], si[m+1]);
	}
};
template<int m>
struct FOR<m, 0> {
	static void unroll(stream_t sr[M+1], stream_t si[M+1]) {}
};

void fft(DTYPE X_R[N], DTYPE X_I[N], DTYPE OUT_R[N], DTYPE OUT_I[N])
{

#pragma HLS interface m_axi port=X_R    bundle=gmem1
#pragma HLS interface m_axi port=X_I    bundle=gmem1
#pragma HLS interface m_axi port=OUT_R  bundle=gmem1
#pragma HLS interface m_axi port=OUT_I  bundle=gmem2

#pragma HLS dataflow

	stream_t stream_R[M+1], stream_I[M+1];

	bit_reverse(X_R, X_I, stream_R[0], stream_I[0]);

//	FOR<M-1, true>::unroll(stream_R, stream_I);
#define FFT_STAGE_SMALL(m) fft_stage_small<m+1>(stream_R[m], stream_I[m], stream_R[m+1], stream_I[m+1])
#define FFT_STAGE_LARGE(m) fft_stage_large<m+1>(stream_R[m], stream_I[m], stream_R[m+1], stream_I[m+1])
//	fft_stage<0>(sr[0], si[0], sr[1], si[1]);
	FFT_STAGE_SMALL(0);
	FFT_STAGE_SMALL(1);
	FFT_STAGE_SMALL(2);
	FFT_STAGE_SMALL(3);
	FFT_STAGE_LARGE(4);
	FFT_STAGE_LARGE(5);
	FFT_STAGE_LARGE(6);
	FFT_STAGE_LARGE(7);
	FFT_STAGE_LARGE(8);
	FFT_STAGE_LARGE(9);

	store_results(stream_R[M], OUT_R);
	store_results(stream_I[M], OUT_I);
}

void bit_reverse(DTYPE X_R[N], DTYPE X_I[N], stream_t &OUT_R, stream_t &OUT_I) {
	DTYPE buff_R[N], buff_I[N];
	for (int i=0; i<N; i++) {
		int rev = 0;
		for (int k=0; k<M; k++) {
			rev = (rev << 1) | ((i >> k) & 1);
		}
		buff_R[rev] = X_R[i];
		buff_I[rev] = X_I[i];
	}
	for (int i=0; i<N; i++) {
		OUT_R.write(buff_R[i]);
		OUT_I.write(buff_I[i]);
	}
}

template<int stage>
void fft_stage_large(stream_t &X_R,  stream_t &X_I, stream_t &OUT_R, stream_t &OUT_I) {
	int m = 1 << stage, mh = 1 << (stage-1);
	int offset = N >> stage;

	DTYPE buff_R[1<<(stage-1)], buff_I[1<<(stage-1)];
	bool start;
	float w_r, w_i, u_r, u_i, a_r, a_i, t_r, t_i;
	start = 0;
	for (int k = 0; k < N; k += m) {
#pragma HLS pipeline off
		int exp = 0;
		for (int j = 0; j < mh; j++) {
#pragma HLS pipeline II=1 rewind
			if (start) {
				OUT_R.write(buff_R[j]);
				OUT_I.write(buff_I[j]);
			}
			buff_R[j] = X_R.read();
			buff_I[j] = X_I.read();
		}
		for (int j = 0; j < mh; j++) {
#pragma HLS pipeline II=1 rewind
			w_r = W_real[exp], w_i = W_imag[exp];
			u_r = buff_R[j], u_i = buff_I[j];
			a_r = X_R.read(), a_i = X_I.read();
			t_r = (w_r * a_r - w_i * a_i);
			t_i = (w_r * a_i + w_i * a_r);
			OUT_R.write(u_r + t_r);
			OUT_I.write(u_i + t_i);
			buff_R[j] = u_r - t_r;
			buff_I[j] = u_i - t_i;
			exp += offset;
		}
		start = 1;
	}
	for (int j = 0; j < mh; j++) {
	#pragma HLS pipeline II=1 rewind
		OUT_R.write(buff_R[j]);
		OUT_I.write(buff_I[j]);
	}
}

template<int stage>
void fft_stage_small(stream_t &X_R,  stream_t &X_I, stream_t &OUT_R, stream_t &OUT_I) {
	int m = 1 << stage, mh = 1 << (stage-1);
	int offset = N >> stage;

	DTYPE buff_R[1<<(stage-1)], buff_I[1<<(stage-1)];
	bool start;
	float w_r, w_i, u_r, u_i, a_r, a_i, t_r, t_i;
	start = 0;
	for (int k = 0; k < N; k += m) {
#pragma HLS pipeline II=1 rewind
		int exp = 0;
		for (int j = 0; j < mh; j++) {
			if (start) {
				OUT_R.write(buff_R[j]);
				OUT_I.write(buff_I[j]);
			}
			buff_R[j] = X_R.read();
			buff_I[j] = X_I.read();
		}
		for (int j = 0; j < mh; j++) {
			w_r = W_real[exp], w_i = W_imag[exp];
			u_r = buff_R[j], u_i = buff_I[j];
			a_r = X_R.read(), a_i = X_I.read();
			t_r = (w_r * a_r - w_i * a_i);
			t_i = (w_r * a_i + w_i * a_r);
			OUT_R.write(u_r + t_r);
			OUT_I.write(u_i + t_i);
			buff_R[j] = u_r - t_r;
			buff_I[j] = u_i - t_i;
			exp += offset;
		}
		start = 1;
	}
	for (int j = 0; j < mh; j++) {
	#pragma HLS unroll
		OUT_R.write(buff_R[j]);
		OUT_I.write(buff_I[j]);
	}
}

void store_results(stream_t &X, DTYPE OUT[N]) {
	DTYPE buff[N];
	for (int i=0; i<N; i++) {
		buff[i] = X.read();
	}
	memcpy(OUT, buff, N * sizeof(DTYPE));
}
