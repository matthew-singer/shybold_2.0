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
    std::normal_distribution<> randomWalk;

    bool saw_last;
    std::vector<double>dist;
    void setPoints(double x_, double y_) { x = x_; y = y_; }

    double distance(const std::shared_ptr<agent> &p) {
        return pow(pow(p->x - x, 2) + pow(p->y - y, 2), .5);
    }
    bool ispred;

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


    void valid_agent(double sensing_radius,  std::shared_ptr<agent> &p) {
      double temp_dist = this->distance(p);
      if (p->alive && temp_dist< sensing_radius) {//eventually change to void
        for(int i=0;i<input_agents; i++){
	  if(dist[i]> temp_dist ){
	    dist.insert(dist.begin()+i, temp_dist);
	    input_agent(input_agent.begin()+i,p);
	    dist.pop_back();
	    input_agent.pop_back();
	    break;


	    
	  }

	}  
       
      
    }

    unsigned long long int survivedTime;
    std::vector<std::shared_ptr<agent> > input_agent;
    
    int lastTime;
    void resetLocation() {
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        alive=true;
    }
    agent(bool _ispred) {
        g = std::make_shared<genome>();
        n = std::make_shared<network>();
	 ispred=_ispred;
      
	
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        lastTime = timeTicks;
        fitness = 0;
        alive = true;
        angle_facing = rates(mutate) * 2 * 3.14159; //random angle facing
        diff_angle = (0.5 * area) / (g->getRadius()*g->getRadius());
        saw_last = false;
        randomWalk = std::normal_distribution<>(0, g->getStddev());
    };
    
    agent(std::shared_ptr<chrome> &p1, std::shared_ptr<chrome> &p2, bool ispred_) {
        g = std::move(std::make_shared<genome>(p1, p2));
        n = std::move(std::make_shared<network>());
        setPoints(rates(mutate) * sizeX, rates(mutate) * sizeY); //set to random
        lastTime = timeTicks;
        fitness = 0;
        alive = true;
	ispred= ispred_;
        angle_facing = rates(mutate) * 2 * 3.14159; //rand angle facing to start
        diff_angle = (0.5 * area) / (g->getRadius()*g->getRadius()); //Theta = A/R2 . It is only half the diff though (plus minus that angle)
        saw_last = false;
        randomWalk = std::normal_distribution<>(0, g->getStddev());
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
        randomWalk(mutate); //mutate is just a randomness varaible - not actually anything to do with mutating
    } 
    int calcFitnessPrey() {
        return lastTime / prey_fitness;
    }

    std::shared_ptr<chrome> getGamete() { return g->mutate_x_over(); }
    std::shared_ptr<agent> set_input(std::vector < std::shared_ptr<agent> >);  
    void output_data(std::fstream &o, bool prey, int gen, int index) ;
    void reset() { 
        if (input_agent.size() < 1) { 
	  for (int i=0; i<input_agents;i++){
            input_agent.push_back(nullptr); 
	    dist.push_back(100000000.0);
	  }
        } else {
            
	  for (int i=0; i<input_agents; i++) {
		input_agent[i]=nullptr;
		
		dist[i]=10000000.0;
            }
        }
    };
};


#endif
