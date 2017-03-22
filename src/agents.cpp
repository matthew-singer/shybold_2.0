#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include "agents.h"
#include <unistd.h>

#define PI 3.14159


    double pick_best(double enemy_c, double c, const int size) {
        if (!USE_TORUS) {
            return enemy_c - c; // defautl condition
        }
        double normal = std::abs(enemy_c - c);
        double add_size_torus = std::abs( (enemy_c - c) + size) ;
        double minus_size_torus = std::abs( (enemy_c - c) - size);
        if (normal < add_size_torus && normal < minus_size_torus) {
            return enemy_c - c;
        } else if (add_size_torus < minus_size_torus) {
            return (enemy_c - c) + size;
        } else { //minus size torus is the winner
            return (enemy_c - c) - size;
        }
    }

    double sqrt_distance(double x, double y) {
        return sqrt(pow(x,2) + pow(y,2));
    }
    void agent::updatePrey() {
        std::vector<double> inputs;
        if (input_agent.size() != 0 && input_agent[0]) { //change this (loop througth input_agents and push_back onto inputs
            saw_last = true;
            double is_x = pick_best(input_agent[0]->x, x, sizeX);
            double is_y = pick_best(input_agent[0]->y, y, sizeY);
            double thet = atan2(is_y, is_x);
            double dist = sqrt_distance(is_x, is_y);
            inputs = { sensing_range_prey / std::max(0.01, dist), thet , double(saw_last), double(!saw_last), angle_facing, 1.0 } ;
        } else {
                saw_last = false;
                inputs = { 0.0, 0.0, double(saw_last), double(!saw_last), angle_facing, 1.0 } ;
        }
        n->update(g, inputs);
        move_mag_theta(n->output_values[0], n->output_values[1], n->output_values[2], pred_capture);
    }

    void agent::updatePred(int time) {
        std::vector<double> inputs;
        
        if (input_agent.size() != 0 && input_agent[0]) { //change this (loop througth input_agents and push_back onto inputs
            saw_last = true;
            double is_x = pick_best(input_agent[0]->x, x, sizeX);
            double is_y = pick_best(input_agent[0]->y, y, sizeY);
            double thet = atan2(is_y, is_x);
            double dist = sqrt_distance(is_x, is_y);
            inputs = { sensing_range_pred / std::max(0.01, dist) ,  thet , double(saw_last), double(!saw_last), angle_facing, 1.0 } ;
           // move_mag_theta(.4, thet, 0, pred_capture);
        } else {
            //make appropariate number of inputs
            saw_last = false;
            inputs = { 0.0, 0.0, double(saw_last), double(!saw_last), angle_facing, 1.0 } ;
        }

        n->update(g, inputs);
        
        move_mag_theta(n->output_values[0], n->output_values[1], n->output_values[2], pred_capture);
        if (USE_TORUS) {
            consume_torus(time);
        } else {
            consume(time);
        }
    }

    //Need to change to get sorted vector of agents inputs
    void agent::getNearestAgentPrey(const std::shared_ptr<agent> &a) {

       if (a->alive && valid_agent(a)) {
           input_agent[0] = a;
        } 
    }

    void agent::getNearestAgentPred(const std::shared_ptr<agent> &a) {
       if (valid_agent(a)) {
            input_agent[0] = a;
        }  
   }

    void agent::consume_torus(int time) {
        if (input_agent[0]) {
            double is_x = pick_best(input_agent[0]->x, x, sizeX);
            double is_y = pick_best(input_agent[0]->y, y, sizeY);
            if (   pow(is_x,2) + pow(is_y, 2) < pow(pred_capture, 2) ) {
                input_agent[0]->alive = false;
                input_agent[0]->lastTime = time;
                ++fitness;
            }
       }

    }

    //figure out what you want to do here
    void agent::consume(int time) {
        if (input_agent[0]) {
            if (this->distance(input_agent[0]) < pred_capture) {
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

    void agent::move_mag_theta(double mag, double theta, double direction_facing, double move) {
        /*direction_facing = std::fmod(direction_facing, 2 * PI);
        if (direction_facing < 0 ) {
            direction_facing += 2 * PI;
        }*/

        if (USE_TORUS) { 
            x = std::fmod(cos(theta) * mag * move + x, sizeX);
            y = std::fmod(sin(theta) * mag * move + y, sizeY);
            if (x < 0) {
                x += sizeX; 
            }
            if (y < 0) {
                y += sizeY ;
            }
        } else {
            x = std::max(0.0, std::min(cos(theta) * mag * move + x, sizeX));
            y = std::max(0.0, std::min(sin(theta) * mag * move + y, sizeY));
        }
        angle_facing = std::fmod(theta, 2 * PI);
        if (angle_facing < 0) {
            angle_facing +=  2 * PI;
        } 
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
