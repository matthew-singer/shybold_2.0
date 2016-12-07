#include <iostream>
#include <algorithm>
#include <fstream>

#include "agents.h"

void agent::updatePrey() {
    std::vector<double> inputs;
    if (input_agent.size() != 0 && input_agent[0]) { //change this (loop througth input_agents and push_back onto inputs
        inputs = {( 10*((input_agent[0])->x - x))/sensing_range_prey    , (10*((input_agent[0])->y - y))/sensing_range_prey, 0.0, 1.0};
    } else {
        //make appropariate number of inputs
        inputs = { 0.0, 0.0, 0.0, 1.0};
    }
    //you hand it chrome 1 values and chrome 2 values from genome
    n->update( g, inputs);
    //move based on ouptputs
    move_x_y(n->output_values[0] * pred_capture,  n->output_values[1] * pred_capture);
}

void agent::updatePred() {
    std::vector<double> inputs;
    if (input_agent.size() != 0 && input_agent[0]) { //change this (loop througth input_agents and push_back onto inputs
        inputs = {( 10*((input_agent[0])->x - x))/sensing_range_pred    , (10*((input_agent[0])->y - y))/sensing_range_pred, 0.0, 1.0};
    } else {
        //make appropariate number of inputs
        inputs = { 0.0, 0.0, 0.0, 1.0};
    }
    //you hand it chrome 1 values and chrome 2 values from genome
    n->update(g, inputs);
    //move based on ouptputs
    move_x_y(n->output_values[0] * pred_capture,  n->output_values[1] * pred_capture);
}

//Need to change to get sorted vector of agents inputs
void agent::getNearestAgentPrey(const std::shared_ptr<agent> &a) {
    if (input_agent.size() == 0) {
        if (this->distance(a) < sensing_range_prey) {
            input_agent.push_back(a);
        }
    } else if (input_agent[0] && this->distance(a) < this->distance(input_agent[0])) {
        //change it here, sort the input_agents and push them appropriately on
        if (this->distance(a) < sensing_range_prey) {
            input_agent[0] = a;
        }
    }
}

void agent::getNearestAgentPred(const std::shared_ptr<agent> &a) {
    if (input_agent.size() == 0) {
        if (this->distance(a) < sensing_range_pred) {
            input_agent.push_back(a);
        }
    } else if (input_agent[0] && (this->distance(a) < this->distance(input_agent[0]) || !input_agent[0]->alive) ) {
        //same thing here
        if (this->distance(a) < sensing_range_pred) {
            input_agent[0] = a;
        }
    }
}

//figure out what you want to do here
void agent::consume(int time) {
    if (input_agent.size() != 0 && input_agent[0]) {
        if (this->distance(input_agent[0]) < pred_capture && input_agent[0]->alive) {
            input_agent[0]->alive = false;
            input_agent[0]->lastTime = time;
            ++fitness;
        }
    }
}

void agent::move_x_y(double dx, double dy) {
    x = std::max(0.0, std::min(x + dx, sizeX));
    y = std::max(0.0, std::min(y + dy, sizeY));
    
}

void agent::output_data(std::fstream &o, bool prey, int gen, int index) {

    o << gen << '\t';
    o << index << '\t';
    if (prey) {
        o << lastTime << '\t';
    } else {
        o << fitness << '\t';
    }
    
    for (int i = 0; i < geneNN; ++i) {
        o << g->getWeight(i) << '\t';
    }

    o << ((g->c1)->metabolic + (g->c2)->metabolic)/2.0 << '\t';
    o << ((g->c1)->radius + (g->c2)->radius) /2.0 << '\t';
    
    if (prey) {
        o << calcFitnessPrey() << '\n';
    } else {
        o << calcFitnessPred() << '\n';
    }
}
