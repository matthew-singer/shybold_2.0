#ifndef PARAMS_H_
#define PARAMS_H_

#include <random>
#include <cmath>

    const int input  = 5;

    const int output = 3;
    const int hiddenLayerSize = 0;
    const int hiddenLayers = 0;

    const int geneNN = (input*output)*((hiddenLayerSize+1)*(hiddenLayers+1));
    const int geneOther = 2;
    const int geneCount = geneNN + geneOther;

    const int generations = 1000;
    const int replicates = 1000;
    const int timeTicks = 1000;

    const int predator_pop = 1000;
    const int prey_pop = 1000;

    const double mut_mean = 0;
    const double mut_stddev = 1;
    const double mut_rate = 1/(double)geneCount;


    extern std::random_device rd;
    extern std::mt19937 mutate;
    extern std::normal_distribution<> mutator;
    extern std::uniform_real_distribution<> rates;

    inline double normal_dist() {
        return mutator(mutate);
    }

    inline double get_mutation() {
        if (mut_rate < rates(mutate)) {
            return 0;
        }
        return mutator(mutate);
    }


#endif
