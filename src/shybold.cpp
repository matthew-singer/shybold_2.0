
#include <iostream>
#include <memory>
#include "population.h"

std::random_device(rd);
std::mt19937 mutate = std::mt19937(rd());
std::normal_distribution<>mutator = std::normal_distribution<>(mut_mean, mut_stddev);
std::uniform_real_distribution<>rates = std::uniform_real_distribution<>(0, 1);

int main() {

    std::unique_ptr<population> pop;
    pop.reset(new population());
    pop->update();

}
