#ifndef AGENT_H_
#define AGENT_H_

#include <memory>
#include "bio.h"
#include "params.h"
#include "network.h"


#define PI 3.14159
static int idNum;

class agent  {
public:

    std::shared_ptr<genome> g;
    std::shared_ptr<network> n;
    int idTrue;

    bool alive;
    bool prey;
    double fitness;
    int deathtime;
    //can be moved to points.h again
    double x, y;
    double sum_x, sum_y;
    double sq_sum_x, sq_sum_y;

    double sum_x_step, sum_y_step;
    double sq_sum_x_step, sq_sum_y_step;

    double angle_facing, diff_angle;
    std::normal_distribution<> randomWalk;

    bool saw_last;

    void setPoints(double x_, double y_) { x = x_; y = y_; }

    double distance(const std::shared_ptr<agent> &p) {
        return pow(pow(p->x - x, 2) + pow(p->y - y, 2), .5);
    }
    
    bool between_agent(const std::shared_ptr<agent> &p) {
        double tmp2 =  atan2(p->y - y, p->x - x);
        if (tmp2 < 0) {
            tmp2 += 2 * PI;
        }
        double phi = std::fmod(std::abs(angle_facing - tmp2), 2 * PI);
        double diff = phi > PI ? 2 * PI - phi : phi;
        if (diff < diff_angle) {
            return true;
        }
        if (p->y - y == 0 && p->x - x == 0) { //on top of each other, auto eat
            return true;
        }
        return false;
    }
    
    bool valid_agent(const std::shared_ptr<agent> &p) {
        if (p->alive && this->distance(p) < g->getRadius()) {
            if (this->between_agent(p)) {
               if (!input_agent[0]) {
                  return true;
               } else if (this->distance(p) <= this->distance(input_agent[0])) {
                    return true;
               } else { 
                   return false;
               }
            }
        } 
        return false;
    }


    bool valid_agent(double sensing_radius, const std::shared_ptr<agent> &p) {
        if (p->alive && this->distance(p) < sensing_radius) {
            if (!input_agent[0]) {
                return true;
            } else if (this->distance(p) <= this->distance(input_agent[0])) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }

    unsigned long long int survivedTime;
    std::vector<std::shared_ptr<agent> > input_agent;
    
    int lastTime;
    void resetLocation() {
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        alive=true;
    }
    void chance_of_death(int time) {
        if (x > highOfDeath) {
            if (chanceDeath > rates(mutate)) {
                alive = false;
                deathtime = time;
            }
        }
    }
    void calc_stuff() {
        sum_x += x;
        sum_y += y;
        sq_sum_x += (x * x);
        sq_sum_y += (y * y);
    }

    agent() {
        idTrue = idNum++;
        sum_x_step = 0;
        sum_y_step = 0;
        sq_sum_x_step = 0;
        sq_sum_y_step = 0;
        sum_x = 0;
        sum_y = 0;
        sq_sum_x = 0;
        sq_sum_y = 0;
        g = std::make_shared<genome>();
        n = std::make_shared<network>();
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        lastTime = timeTicks;
        fitness = 0;
        alive = true;
        angle_facing = rates(mutate) * 2 * 3.14159; //random angle facing
        diff_angle = (0.5 * area) / (g->getRadius()*g->getRadius());
        saw_last = false;
        randomWalk = std::normal_distribution<>(0, g->getStddev());
        deathtime = timeTicks;
    };
    
    agent(std::shared_ptr<chrome> &p1, std::shared_ptr<chrome> &p2) {
        idTrue = idNum++;
        sum_x = 0;
        sum_y = 0;
        sq_sum_x = 0;
        sq_sum_y = 0;
        
        sum_x_step = 0;
        sum_y_step = 0;
        sq_sum_x_step = 0;
        sq_sum_y_step = 0;
        
        g = std::move(std::make_shared<genome>(p1, p2));
        n = std::move(std::make_shared<network>());
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        lastTime = timeTicks;
        fitness = 0;
        alive = true;
        angle_facing = rates(mutate) * 2 * 3.14159; //rand angle facing to start
        diff_angle = (0.5 * area) / (g->getRadius()*g->getRadius()); //Theta = A/R2 . It is only half the diff though (plus minus that angle)
        saw_last = false;
        randomWalk = std::normal_distribution<>(0, g->getStddev());
        deathtime = timeTicks;
    };
    
    void getNearestAgentPrey(const std::shared_ptr<agent> &a); 
    void getNearestAgentPred(const std::shared_ptr<agent> &a); 

    void updatePred(int time); 
    void updatePrey(); 
    void move_x_y(double dx, double dy);
    void move_mag_theta(double mag, double theta, double direction_facing, double move);

    void consume(int time);

    int calcFitnessPred() {
        return fitness * pred_fitness;    
    }
    double getRandom() {
        return randomWalk(mutate); //mutate is just a randomness varaible - not actually anything to do with mutating
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
