#include "VPEArray2.h"
#include "common.h"
#include <functional>
#include <string>

struct DUT : public Module<VPEArray2> {
  template <typename T, int N, int K>
  void loadStationaryData(T m[N][K]) {
    loadStationaryData<T, N, K>(
      [&](int i, int k) {return m[i][k];}
    );
  }

  template <typename T, int N, int K>
  void loadStationaryData(std::function<T(int i,int k)> f) {
    top->stationaryCtrl = 1;
    top->b_in = 0;
    top->c_in = 0;
    for (int t=0;t<N;t++) {
      poke_array<K, u8>(top->a_in, [&](int k) {
        int i = N-1-t; return f(i,k);
      });
      tick();
    }
    top->stationaryCtrl = 0;
    top->a_in = 0;
  }
};

template<int N, int M, int K> int test() {
  static_assert(N % 4 == 0);
  static_assert(K % 4 == 0);
  int result = TEST_FAIL;
  Data<u8, N, M, K> data;

  std::string case_name = std::to_string(N) + "x" + std::to_string(M) + "x" + std::to_string(K);
  printf("Running test for %s\n", case_name.c_str());
  if (verbose) data.dump_np();

  for (int shift = 0; shift < 5; shift ++) {
    DUT dut;
    int this_result = TEST_PASS; 
    auto vcd_file = tb_name + "." + case_name + "." + std::to_string(shift) + ".vcd";
    dut.open_trace(vcd_file);

    u8 partial[N][M], output[N][M];
    for (int i=0;i<N;i++)
      for(int j=0;j<M;j++)
        partial[i][j] = output[i][j] = 0;

    dut.reset();

    const int NP = N/4, KP = K/4;
    for (int i2 = 0; i2 < NP; i2++)
      for (int k2 = 0; k2 < KP; k2++) {
        // prepare for partial
        for (int j=0;j<M;j++) {
          for (int i=i2*4;i<(i2+1)*4;i++) {
            for (int k=k2*4;k<(k2+1)*4;k++)
              partial[i][j] += data.a[i][k] * data.b[k][j];
            partial[i][j] &= 0xff;
          }
        }

        dut.loadStationaryData<u8,4,4>( [&](int i,int k){ return data.a[i2*4+i][k2*4+k]; });

        for (int t=0;t<M+std::max(2,shift);t++) {
          if (t < M) {
            poke_array<4,u8>(dut->b_in, [&](int k) {
              return data.b[k2*4+k][t];
            });
          } else dut->b_in = 0;

          {
            int t2 = t - shift;
            if (0<=t2 && t2<M) {
              poke_array<4,u8>(dut->c_in, [&](int i) {
                return output[i2*4+i][t2];
              });
            } else dut->c_in = 0;
          }

          if (t >= 3) { // latency of addtree of 5
            auto c_out = peek_array<4, u8>(dut->c_out);
            // j = t
            int j = t - 3;
            if (j < M) {
              for (int i=0;i<4;i++) {
                output[i2*4+i][j] = c_out[i];
                if (output[i2*4+i][j] != partial[i2*4+i][j])
                  this_result = TEST_FAIL;
              }
            }
          }
          dut.tick();
        }
    }
    if (this_result == TEST_PASS) {
      result = TEST_PASS;
      break;
    }
  }
  return result;
}

void run(std::function<int()> f) {
  printf("Result: %s\n", f() ? "\e[91mFAIL\e[0m" : "\e[92mPASS\e[0m");
}

int main(int argc, char* argv[]) {
  init_ctx(argc, argv);
  run(test<4,4,4>);
  run(test<4,8,4>);
  run(test<8,4,4>);
  run(test<8,8,8>);
  run(test<16,9,16>);
  return 0;
}