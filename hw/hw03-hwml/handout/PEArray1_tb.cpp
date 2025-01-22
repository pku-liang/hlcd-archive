#include "VPEArray1.h"
#include "common.h"
#include <functional>
#include <string>

struct DUT : public Module<VPEArray1> {
  template <typename T, int M, int K>
  void loadStationaryData(T m[K][M]) {
    loadStationaryData<T, M, K>(
      [&](int k, int j) {return m[k][j];}
    );
  }

  template <typename T, int M, int K>
  void loadStationaryData(std::function<T(int k,int j)> f) {
    top->stationaryCtrl = 1;
    top->a_in = 0;
    top->c_in = 0;
    for (int t=0;t<K;t++) {
      poke_array<M, u8>(top->b_in, [&](int j) {
        int k = K-1-t; return f(k,j);
      });
      tick();
    }
    top->stationaryCtrl = 0;
    top->b_in = 0;
  }
};

template<int N, int M, int K> int test() {
  static_assert(M % 4 == 0);
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

    const int MP = M/4, KP = K/4;
    for (int j2 = 0; j2 < MP; j2++)
      for (int k2 = 0; k2 < MP; k2++) {
        // prepare for partial
        for (int i=0;i<N;i++) {
          for (int j=j2*4;j<(j2+1)*4;j++) {
            for (int k=k2*4;k<(k2+1)*4;k++)
              partial[i][j] += data.a[i][k] * data.b[k][j];
            partial[i][j] &= 0xff;
          }
        }

        dut.loadStationaryData<u8,4,4>( [&](int k,int j){ return data.b[k2*4+k][j2*4+j]; });

        for (int t=0;t<N+7;t++) {
          if (t < N + 3) {
            poke_array<4, u8>(dut->a_in, [&](int k) {int i = t - k; 
              return (0<=i && i<N) ? data.a[i][k2*4+k] : 0;
            });
          } else dut->a_in = 0;
          {
            int t2 = t - shift;
            if (0 <= t2 && t2 < N+3) {
              poke_array<4, u8>(dut->c_in, [&](int j) {
                int i = t2-j;
                return (0<=i && i<N) ? output[i][j2*4+j] : 0;
              });
            } else dut->c_in = 0;
          }
          if (t > 4) {
            auto c_out = peek_array<4, u8>(dut->c_out);
            for (int j=0;j<4;j++) {
              int i = t-5-j;
              if (0<=i && i<N) {
                output[i][j2*4+j] = c_out[j];
                if (output[i][j2*4+j] != partial[i][j2*4 + j])
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
  run(test<8,4,4>);
  run(test<8,8,8>);
  run(test<9,16,16>);
  return 0;
}