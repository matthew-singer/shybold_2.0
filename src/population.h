#ifndef POPULATION_H_
#define POPULATION_H_

#include <vector>
#include <memory>
#include <iostream>
#include "params.h"
#include "agents.h"

class population {
public:
    std::array< std::shared_ptr<agent>, predator_pop_count * replicates > preds_pop;
    std::array< std::shared_ptr<agent>, prey_pop_count * replicates > prey_pop;
    
    int time;
    int gen;

    population() {
        time = timeTicks;
        gen = 0;
        for (int i = 0; i < predator_pop_count * replicates ; ++i) {
            preds_pop[i] = std::make_shared<agent>();
        }
        for (int i = 0; i < prey_pop_count * replicates ; ++i) {
            prey_pop[i] = std::make_shared<agent>();
        }
    }
    void update();
    template<std::size_t SIZE>
    void reproduce(std::vector< std::shared_ptr<chrome> > &gametes, std::array< std::shared_ptr<agent>, SIZE> &pop, int pop_size );

    void nextGen(std::vector<chrome> pred, std::vector<chrome> prey) { //do i use this funciton?
        ++gen;
    }


};




#endif
