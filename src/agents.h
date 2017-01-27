#ifndef AGENT_H_
#define AGENT_H_

#include <memory>
#include "bio.h"
#include "params.h"
#include "network.h"


#define PI 3.14159

class agent  {
public:

    std::shared_ptr<genome> g;
    std::shared_ptr<network> n;

    bool alive;
    bool prey;
    double fitness;

    //can be moved to points.h again
    double x, y;
    double angle_facing, diff_angle;
    
    bool saw_last;

    void setPoints(double x_, double y_) { x = x_; y = y_; }

    double distance(const std::shared_ptr<agent> &p) {
        return pow(pow(p->x - x, 2) + pow(p->y - y, 2), .5);
    }
    
    bool between_agent(const std::shared_ptr<agent> &p) {
        angle_facing = std::fmod(angle_facing, 2 * PI);
        if (angle_facing < 0) {
            angle_facing += 2 * PI;
        }
       
        if (std::abs(angle_facing - atan2(p->y - y, p->x - x)) < diff_angle) {
            return true;
        }
        if (p->y - y == 0 && p->x - x == 0) { //on top of each other, auto eat
            return true;
        }
        return false;
    }
    
    bool valid_agent(const std::shared_ptr<agent> &p) {
        if (this->distance(p) < g->getRadius()) {
            if (this->between_agent(p)) {
                return true; 
            }
        }
        return false;
    }

    unsigned long long int survivedTime;
    std::vector<std::shared_ptr<agent> > input_agent;

    int lastTime;

    agent() {
        g = std::make_shared<genome>();
        n = std::make_shared<network>();
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        lastTime = timeTicks;
        fitness = 0;
        alive = true;
        angle_facing = rates(mutate) * 2 * 3.14159; //random angle facing
        diff_angle = (2.0 * area) / (g->getRadius()*g->getRadius());
    };
    
    agent(std::shared_ptr<chrome> &p1, std::shared_ptr<chrome> &p2) {
        g = std::make_shared<genome>(p1, p2);
        n = std::make_shared<network>();
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        lastTime = timeTicks;
        fitness = 0;
        alive = true;
        angle_facing = rates(mutate) * 2 * 3.14159; //rand angle facing to start
        diff_angle = (0.5 * area) / (g->getRadius()*g->getRadius()); //Theta = A/R2 . It is only half the diff though (plus minus that angle)
    };
    
    void getNearestAgentPrey(const std::shared_ptr<agent> &a); 
    void getNearestAgentPred(const std::shared_ptr<agent> &a); 

    void updatePred(); 
    void updatePrey(); 
    void move_x_y(double dx, double dy);
    void move_mag_theta(double mag, double theta, double direction_facing);

    void consume(int time);

    int calcFitnessPred() {
        return fitness * pred_fitness;    
    }
    
    int calcFitnessPrey() {
        return lastTime / prey_fitness;
    }

    std::shared_ptr<chrome> getGamete() { return g->mutate_x_over(); }
    std::shared_ptr<agent> set_input(std::vector < std::shared_ptr<agent> >);  
    void output_data(std::fstream &o, bool prey, int gen, int index) ;
    void reset() { 
        if (input_agent.size() < 1) { 
            input_agent.push_back(nullptr); 
        } else {
            input_agent[0] = nullptr;
            /*for (auto &i : input_agent) {
                i = nullptr;
            }*/
        }
    };
};


#endif
