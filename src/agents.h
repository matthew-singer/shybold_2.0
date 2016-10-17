#ifndef AGENT_H_
#define AGENT_H_

#include <memory>
#include "bio.h"
#include "params.h"
#include "network.h"
#include "points.h"

class agent : public points {
public:

	genome g;
    network n;

	bool alive;
    bool prey;
    double fitness;

	unsigned long long int survivedTime;
    std::vector<std::shared_ptr<agent> > input_agent;
    
    int lastTime;

    agent() {
        setPoints(rander(mutate), rander(mutate)); //set to random
        lastTime = timeTicks;
        for (int i = 0; i < input_agents; ++i) {
            input_agent.push_back(nullptr);
        }
        fitness = 0;
    };
    
    agent(std::shared_ptr<chrome> p1, std::shared_ptr<chrome> p2) {
        setPoints(rander(mutate), rander(mutate)); //set to random
        lastTime = timeTicks;
        for (int i = 0; i < input_agents; ++i) {
            input_agent.push_back(nullptr);
        }
        fitness = 0;
    };
    
    void getNearestAgent(std::shared_ptr<agent> a); 
    void update(); 
    void move_x_y(double dx, double dy);
    void move_theta_mag(double theta, double mag);

    void consume(int time) { 
        if (this->distance(input_agent[0]) < pred_capture) {
            input_agent[0]->alive = false;
            input_agent[0]->lastTime = time;  
            ++fitness;
        }
    }

    int calcFitnessPred() {
        return fitness * pred_fitness;    
    }
    
    int calcFitnessPrey() {
        return lastTime / prey_fitness;
    }

    std::shared_ptr<chrome> getGamete() { return g.mutate_x_over(); }
    std::shared_ptr<agent> set_input(std::vector < std::shared_ptr<agent> >);  
    void output_data(std::fstream &o, bool prey, int gen, int index) ;

};


#endif
