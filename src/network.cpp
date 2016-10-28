#include "network.h"

void network::update(std::array<double, geneNN> val1, std::array<double, geneNN> val2, std::vector<double> inputs) {
    for (int o = 0; o < output_values.size(); ++o) {
        output_values[o] = 0;
        for (int i = 0; i < inputs.size(); ++i) {
            //std::cout << "Input Output Calc " << i << " " << o << " " << (o*(inputs.size()))+i << '\n';
            output_values[o] += (inputs[i] * ((val1[(o*(inputs.size()-1))+i] + val2[(o*(inputs.size()-1))+i])/2.0)  );
        }
        output_values[o] = -1.0 + 2.0 * (1.0/(1.0+pow(2.7183,-1*output_values[o])));
    }    
}
