#ifndef AGENT_H_
#define AGENT_H_

#include <memory>
#include "bio.h"

class agent {
public:

	genome g;
	bool alive;
	unsigned long long int survivedTime;
    std::shared_ptr<agent> input_agent;

    agent() {};
    
    void move() { }    ;

    std::shared_ptr<agent> set_input(std::vector < std::shared_ptr<agent> >);
    

};



#endif
