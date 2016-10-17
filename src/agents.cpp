#include <iostream>
#include <algorithm>
#include <fstream>

#include "agents.h"

void agent::update() {
    if (alive) { //double cehck
        //calc nn
        std::vector<double> inputs = {(input_agent[0])->x, (input_agent[0])->y};
        n.update( (g.c1)->values , (g.c2)->values, inputs);
        //move based on nn outputs
        move_x_y(n.output_values[0] ,  n.output_values[1]);
    }
}

//Need to change to get sorted vector of agents inputs
void agent::getNearestAgent(std::shared_ptr<agent> a) {
    if (!input_agent[0]) {
        input_agent[0] = a;
    } else if (this->distance(a) < this->distance(input_agent[0])) {
        input_agent[0] = a;
    }
}

void agent::move_x_y(double dx, double dy) {
    x += dx;
    y += dy;
}

void agent::output_data(std::fstream &o, bool prey, int gen, int index) {


    o << gen << '\t';
    o << index << '\t';
    if (prey) {
        o << survivedTime << '\t';
    } else {
        o << fitness << '\t';
    }
    
    for (int i = 0; i < geneNN; ++i) {
        o << (g.c1->values[i] + g.c2->values[i])/2.0 << '\t';
    }

    o << (g.c1->metabolic + g.c2->metabolic)/2.0 << '\t';
    o << (g.c1->radius + g.c2->radius) /2.0 << '\t';
    
    if (prey) {
        o << calcFitnessPrey() << '\n';
    } else {
        o << calcFitnessPred() << '\n';
    }
}
