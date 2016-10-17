#include "network.h"

void network::update(std::array<double, geneNN> val1, std::array<double, geneNN> val2, std::vector<double> inputs) {
    for (int o = 0; o < geneNN; ++o) {
        output_values[o] = 0;
        for (int i = 0; i < input; ++i) {
            output_values[o] += (inputs[i] * ((val1[(o*i)+i] + val2[(o*i)+i])/2.0)  );
        }
        output_values[o] = -1.0 + 2.0 * (1.0/(1.0+pow(2.7183,-1*output_values[o])));
    }    
}
