#include "mm.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cassert>

in_t A[N][N];
in_t B[N][N];
data_t C[N][N];
float gold[N][N];
int num_fail = 0;

inline void fchk(float a, float b, int i, int j, const char * mat) {
    if(std::abs(a - b) >= eps) {
        num_fail ++;
        std::cout << "Error at mat " << mat << " (" << i << ", " << j << ") output=" << a << " gold=" << b << std::endl;
    }
}

int main() {
    std::ifstream data("data.txt");
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            float x; data >> x;
            A[i][j] = x;
            fchk(A[i][j], x, i, j, "A");
        }
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            float x; data >> x;
            B[i][j] = x;
            fchk(B[i][j], x, i, j, "A");
        }
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            data >> gold[i][j];
        }
    }
    mm(A, B, C);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            fchk(C[i][j], gold[i][j], i, j, "C");
        }
    }
    if(num_fail) {
        std::abort();
    } else {
        std::cout << "SUCCESS!" << std::endl;
    }
    return 0;
}
