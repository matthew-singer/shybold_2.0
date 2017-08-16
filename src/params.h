#ifndef PARAMS_H_
#define PARAMS_H_

#include <random>
#include <cmath>
#include <iostream>
#include <fstream>

#ifdef OPEN_CV
#include<opencv2/opencv.hpp>
const std::string windowName = "AhYis";
#endif
    const int input  = 3; //bias node and delta x and delta y
    const int input_agents = 1; // number of agents to input into the NN

//Parameters for the predation function

    const double i_s = 0;
    const double r_s = 0.01;
    const double c_s = 500.0;
    const double a_s = 0.0;

    const int replicates = 1000; //number of tanks
    const int predator_pop_count = 1;
    const int prey_pop_count = 20; //turn off prey nn

    const int output = 3;
    const int hiddenLayerSize = 0;
    const int hiddenLayers = 0;
    
    const int geneNN = ((input+input_agents*2)*output)*((hiddenLayerSize+1)*(hiddenLayers+1));
    
    const double area = 50000;

    const int generations = 1000;
    const int timeTicks = 3000;


    const double pred_capture = 8; 
    const double sensing_range_pred = 125;
    const double sensing_range_prey = 200;
    const double base_sensing_range = 100;

    const double sizeX = 1000;
    const double sizeY = 1000;

    const double mut_mean = 0;
    const double mut_stddev = 1.0;
    const double mut_rate = 1/(double)geneNN;

    const double xover_rate = .5;

    const double pred_fitness = 2;

    const double prey_fitness = 250.0;

    extern std::fstream output_file_prey;
    extern std::fstream output_file_pred;

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

    const int breakupX = 10;
    const int breakupY = 10;



#endif

