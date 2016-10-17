#ifndef NETWORK_H_
#define NETWORK_H_

#include <array>
#include <vector>
#include "params.h"

class network {
public:
    std::array<double, output> output_values;
    network() { };
    void update(std::array<double, geneNN> val1,std::array<double, geneNN> val2, std::vector<double> inputs);
    //void update(std::array<double, geneNN> val, std::vector<double> inputs);
};

#endif


