#ifndef AGENT_H_
#define AGENT_H_

#include <memory>
#include "bio.h"
#include "params.h"
#include "network.h"

class agent  {
public:

    std::shared_ptr<genome> g;
    std::shared_ptr<network> n;

	bool alive;
    bool prey;
    double fitness;

    //can be moved to points.h again
    double x, y;
    void setPoints(double x_, double y_) { x = x_; y = y_; }

    double distance(const std::shared_ptr<agent> &p) {
        return pow(p->x - x, 2) + pow(p->y - y, 2);
    }

	unsigned long long int survivedTime;
    std::vector<std::shared_ptr<agent> > input_agent;

    int lastTime;

    agent() {
        g.reset(new genome());
        n.reset(new network());
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        lastTime = timeTicks;
        fitness = 0;
        alive = true;
    };
    
    agent(std::shared_ptr<chrome> &p1, std::shared_ptr<chrome> &p2) {
        g.reset(new genome(p1, p2));
        n.reset(new network());
        setPoints(rander(mutate) * sizeX, rander(mutate) * sizeY); //set to random
        lastTime = timeTicks;
        fitness = 0;
        alive = true;
    };
    
    void getNearestAgentPrey(const std::shared_ptr<agent> &a); 
    void getNearestAgentPred(const std::shared_ptr<agent> &a); 

    void update(); 
    void move_x_y(double dx, double dy);
    void move_theta_mag(double theta, double mag);

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

};


#endif
