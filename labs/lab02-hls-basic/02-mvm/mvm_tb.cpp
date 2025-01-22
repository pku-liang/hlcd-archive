#include "mvm.h"
#include <fstream>
#include <iostream>
#include <cmath>

static mat_t mat;
static vec_t vec;
static res_t res;
static float gold[N];

int main() {
    std::ifstream fmat("mat.txt");
    std::ifstream fvec("vec.txt");
    std::ifstream fgold("res.txt");

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            float data; fmat >> data;
            mat[i][j] = data;
        }
    }
    for(int i = 0; i < N; i++) {
        float data; fvec >> data;
        vec[i] = data;
    }
    for(int i = 0; i < N; i++) {
        fgold >> gold[i];
    }

    mvm(mat, vec, res);

    int num_fail = 0;
    for(int i = 0; i < N; i++) {
        float out = res[i];
        if(std::abs(out - gold[i]) >= 1e-4) {
            std::cout << "Error at " << i << " output=" << out << " gold=" << gold[i] << std::endl;
            num_fail++;
        }
    }
    
    if(!num_fail) {
        std::cout << "Success!" << std::endl;
    }

    return num_fail;
}