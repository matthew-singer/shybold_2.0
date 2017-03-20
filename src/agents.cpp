#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include "agents.h"
#include <thread>

#define PI 3.14159

void agent::updatePrey() {
    std::vector<double> inputs;
    if (input_agent.size() != 0 && input_agent[0]) { //change this (loop througth input_agents and push_back onto inputs
        saw_last = true;
        inputs = { g->getRadius() / std::max(0.01, this->distance(input_agent[0])),  atan2(input_agent[0]->y - y, input_agent[0]->x - x) , double(saw_last), double(!saw_last), angle_facing, 1.0 } ;
    } else {
        saw_last = false;
        inputs = { 0.0, 0.0, double(saw_last), double(!saw_last), angle_facing, 1.0 } ;
        }
        n->update(g, inputs);
        //move_mag_theta(n->output_values[0], n->output_values[1], n->output_values[2], pred_capture);
       if (input_agent[0]) {
            double is_x = std::abs(input_agent[0]->x - x) < sizeX - std::abs(input_agent[0]->x - x) ? input_agent[0]->x - x : sizeX - std::abs(input_agent[0]->x - x);
            double is_y = std::abs(input_agent[0]->y - y) < sizeY - std::abs(input_agent[0]->y - y) ? input_agent[0]->y - y : sizeY - std::abs(input_agent[0]->y - y);
            //double is_y = std::min(std::abs(input_agent[0]->y - y), sizeY - std::abs(input_agent[0]->y - y));
            double thet = atan2(is_y, is_x);

            //std::cout << atan2(is_y, is_x) << std::endl;
            if (thet < 0) {
                thet += 2 * PI;
            }
            std::cout << x << "\t" <<  y << std::endl;
            move_mag_theta(-.3, thet, 0, pred_capture);
       }
    }

    void agent::updatePred(int time) {
        std::vector<double> inputs;
        
        if (input_agent.size() != 0 && input_agent[0]) { //change this (loop througth input_agents and push_back onto inputs
            saw_last = true;
            
            inputs = { g->getRadius() / std::max(0.01, this->distance(input_agent[0])) ,  atan2(input_agent[0]->y - y, input_agent[0]->x - x) , double(saw_last), double(!saw_last), angle_facing, 1.0 } ;
        } else {
            //make appropariate number of inputs
            saw_last = false;
            inputs = { 0.0, 0.0, double(saw_last), double(!saw_last), angle_facing, 1.0 } ;
        }
        //you hand it chrome 1 values and chrome 2 values from genome
        n->update(g, inputs);

       if (input_agent[0]) {
            double is_x = std::abs(input_agent[0]->x - x) < sizeX - std::abs(input_agent[0]->x - x) ? input_agent[0]->x - x : sizeX - (input_agent[0]->x - x);
            double is_y = std::abs(input_agent[0]->y - y) < sizeY - std::abs(input_agent[0]->y - y) ? input_agent[0]->y - y : sizeY - (input_agent[0]->y - y);
            //double is_y = std::min(std::abs(input_agent[0]->y - y), sizeY - std::abs(input_agent[0]->y - y));
            move_mag_theta(.3, atan2(is_y, is_x), 0, pred_capture);
             
       } 

       // move_mag_theta(n->output_values[0], n->output_values[1], n->output_values[2], pred_capture);
        consume(time);

    }

    //Need to change to get sorted vector of agents inputs
    void agent::getNearestAgentPrey(const std::shared_ptr<agent> &a) {

       if (a->alive && valid_agent(a)) {
           input_agent[0] = a;
        } 
        /*
        if (input_agent.size() == 0) {
            if (this->distance(a) < sensing_range_prey ) {
                input_agent.push_back(a);
            }
        } else if (!input_agent[0] || this->distance(a) < this->distance(input_agent[0])) {
            //change it here, sort the input_agents and push them appropriately on
            if (this->distance(a) < sensing_range_prey) {
                input_agent[0] = a;
            }
        }*/
    }

    void agent::getNearestAgentPred(const std::shared_ptr<agent> &a) {
       if (valid_agent(a)) {
            input_agent[0] = a;
        }  
       /*if (input_agent.size() == 0) {
            if (this->distance(a) < sensing_range_pred) {
                input_agent.push_back(a);
            }
        } else if ( !input_agent[0] || this->distance(a) < this->distance(input_agent[0]) || !input_agent[0]->alive ) {
            //same thing here
            if (this->distance(a) < sensing_range_pred) {
                input_agent[0] = a;
            }
        }*/
    }

    //figure out what you want to do here
    void agent::consume(int time) {
        if (input_agent[0]) {
            if (this->distance(input_agent[0]) < pred_capture) {
                input_agent[0]->alive = false;
                input_agent[0]->lastTime = time;
                ++fitness;
            }
            /*if (this->distance(input_agent[0]) < pred_capture && input_agent[0]->alive) {
                input_agent[0]->alive = false;
                input_agent[0]->lastTime = time;
                ++fitness;
            }*/
        }
    }

    void agent::move_x_y(double dx, double dy) {
        x = std::max(0.0, std::min(x + dx, sizeX));
        y = std::max(0.0, std::min(y + dy, sizeY));
    }

    void agent::move_mag_theta(double mag, double theta, double direction_facing, double move) {
    /*std::cout << "X " << sin(theta) * mag << '\n';
    std::cout << "Y " << cos(theta) * mag << '\n';
    std::cout << "Theta " << theta << '\n';
    std::cout << "Mag " << mag << '\n';*/
    direction_facing = std::fmod(direction_facing, 2 * PI);
    if (direction_facing < 0 ) {
        direction_facing += 2 * PI;
    }
    //x = std::max(0.0, std::min(cos(theta) * mag * move + x, sizeX));
    //y = std::max(0.0, std::min(sin(theta) * mag * move + y, sizeY));
    
    x = std::fmod(cos(theta) * mag * move + x, sizeX);
    if (x < 0) {
        x += sizeX; 
    }
    y = std::fmod(sin(theta) * mag * move + y, sizeY);
    if (y < 0) {
        y += sizeY ;
    }
    //angle_facing = std::fmod(angle_facing + direction_facing, 2 * PI);
    angle_facing = direction_facing;
    
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
