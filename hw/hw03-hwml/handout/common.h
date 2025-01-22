#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <ctime>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <functional>
#include "verilated.h"
#include "verilated_vcd_c.h"

std::string tb_name;
bool verbose = false;

void init_ctx(int argc, char** argv) {
    tb_name = argv[0];
    printf("TB: %s\n", tb_name.c_str());

    if (argc > 1) {
        if (!strcmp("-v", argv[1])) {
            verbose = true;
        }
    }
}

enum TEST_RESULT {
    TEST_PASS = 0,
    TEST_FAIL = 1
};

// Matrix Data generator
template<int n, int m, typename T>
void dump_array(const char* name, T a[n][m], bool np=false) {
    printf(np?"%s=np.array([":"%s:\n", name);

    for (int i=0;i<n;i++) {
        if (np) printf("[");
        for (int j=0;j<m;j++) {
            printf("%d%c", a[i][j], np ? ',' : '\t');
        }
        printf(np?"],":"\n");
    }

    if (np) printf("])\n");
}
template<typename T, int n, int m, int k>
struct Data {
    T a[n][k], b[k][m], c[n][m];
    Data() {
        int W = sizeof(T)*8;
        T input_mask = (1<<(W/2))-1,
          output_mask = (1<<(W))-1;
        srand(time(NULL));

        for (int i=0;i<n;i++) {
            for (int j=0;j<k;j++) {
                a[i][j] = rand() & input_mask;
            }
        }

        for (int i=0;i<k;i++) {
            for (int j=0;j<m;j++) {
                b[i][j] = rand() & input_mask;
            }
        }

        for (int i=0;i<n;i++) {
            for (int j=0;j<m;j++) {
                c[i][j] = 0;
                for (int l=0;l<k;l++) {
                    c[i][j] += a[i][l] * b[l][j];
                }
                c[i][j] &= output_mask;
            }
        }
    }
    void dump() {
        dump_array<n,k>("a", a);
        dump_array<k,m>("b", b);
        dump_array<n,m>("c", c);
    }
    void dump_np() {
        dump_array<n,k>("a", a, true);
        dump_array<k,m>("b", b, true);
        dump_array<n,m>("c", c, true);
    }
};

// Array Data Packing Helpers
template<int n, typename E, typename F>
void poke_array(F& field, std::function<E(int)> f) {
    static_assert(sizeof(F) == sizeof(E)*n);
    F value = 0;
    for (int i=0;i<n;i++)
        value |= f(i) << (sizeof(E)*8*i);
    field = value;
}
template<int n, typename E, typename F>
std::array<E,n> peek_array(F& field) {
    static_assert(sizeof(F) == sizeof(E)*n);
    int sz = sizeof(E)*8;
    int mask = (1<<sz)-1;
    std::array<E,n> ret;
    for (int i=0;i<n;i++) {
        ret[i] = (field >> (sz*i) & mask);
    }
    return ret;
}

typedef uint8_t u8;
template<typename T>
struct Module {
    T* top;
    VerilatedContext* contextp = nullptr;
    VerilatedVcdC* tfp = nullptr;
    uint64_t t;


    Module() {
        t = 0;
        contextp = new VerilatedContext;
        contextp->traceEverOn(true);
        top = new T{contextp, "TOP"};
        tfp = new VerilatedVcdC;
        top->trace(tfp, 99);
    }
    ~Module() {
        top->clk = 1;
        tfp->dump(t++);
        tfp->close();
        delete tfp;
        delete top;
        delete contextp;
    }

    void open_trace(std::string trace_name) {
        tfp->open(trace_name.c_str());
        tfp->dump(t++);
    }

    void tick() {
        top->clk = 1;
        top->eval();
        tfp->dump(t++);
        top->clk = 0;
        top->eval();
        tfp->dump(t++);
    }
    void reset() {
        top->rst = 1;
        tick();
        top->rst = 0;
    }
    T* operator -> () {
        return top;
    }
};
#endif