#include "fir.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <cstdlib>

int main () {
    system("pwd");
    std::ifstream fin("data.txt");
    std::ifstream fgold("gold.txt");
    std::vector<data_t> data;
    std::vector<acc_t> gold, output;
    int x;
    while(fin >> x) data.push_back(data_t(x));
    while(fgold >> x) gold.push_back(acc_t(x));
    std::cout << "input_size=" << data.size() << std::endl;
    assert(gold.size() == data.size() && "Invalid Gold Output");
    for(auto & x: data) {
        acc_t acc;
        fir(&acc, x);
        output.push_back(acc);
    }
    int num_fail = 0;
    for(size_t i = 0; i < gold.size(); i++) {
        if(output[i] != gold[i]) {
            std::cout << "Error at " << i << " output=" << output[i] << " gold=" << gold[i] << std::endl;
            num_fail++;
        }
    }
    if(!num_fail) {
        std::cout << "Success!" << std::endl;
    }
    return num_fail;
}

