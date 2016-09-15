#ifndef PARAMS_H_
#define PARAMS_H_

#include <random>
#include <cmath>

int input = 3;
int output = 3;
int hiddenLayerSize = 0;
int hiddenLayers = 0;

int geneCount = (input*output)*((hiddenLayerSize+1)*(hiddenLayers+1));

int generations = 1000;
int replicates = 1;
int timeTicks = 1000;


double mut_mean = 0;
double mut_stddev = 1;
double mut_rate = 1/(double)geneCount;

std::random_device rd;
std::mt19937 mutate(rd());
std::normal_distribution<>mutator(mut_mean, mut_stddev);
std::uniform_real_distribution<>rates(0, 1); 

double get_mutation() {
	if (mut_rate < rates(mutate)) {
		return 0;
	}
	return mutator(mutate);
}

#endif
