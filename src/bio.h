#ifndef BIO_H_
#define BIO_H_

#include <vector>
#include <array>
#include <memory>
#include "params.h"


class chrome {
public:
    double metabolic;
    double radius;
    //other genes here
    int id;

    std::array<double, geneNN> values;
    chrome(std::shared_ptr<chrome> base) {
        for (int i = 0; i < values.size(); ++i) {
            values[i] = (base->values)[i] + get_mutation();
        }
        radius += (base->radius) + get_mutation();
        metabolic += (base->metabolic) +  get_mutation();
    }	

    chrome() {
        for (int i = 0; i < values.size(); ++i) {
           values[i] = normal_dist();
        }
        radius = normal_dist();
        metabolic = normal_dist();
    } 

    template<class T>
    void setValue(T &v, T p1, T p2) {
        if (rates(mutate) < xover_rate) {
            v = p1;
        } else {
            v = p2;
        }
    }

    std::shared_ptr<chrome> xover_mut(std::shared_ptr<chrome> p2) {
        std::shared_ptr<chrome> c = std::make_shared<chrome>(); //should make "clean" constructor
        for (int i = 0; i < values.size(); ++i) {
                setValue((c->values)[i], (this->values)[i], (p2->values)[i]);
                (c->values)[i] += get_mutation(); //want to keep set value pure
        }
        setValue(c->radius, this->radius, p2->radius);
        c->radius += get_mutation(); 
        setValue(c->radius, this->radius, p2->radius);
        c->metabolic += get_mutation(); 

        return c;
    }

};

class genome {
public:
	std::shared_ptr<chrome> c1;
	std::shared_ptr<chrome> c2;
    	
    genome(std::shared_ptr<chrome> c1_, std::shared_ptr<chrome> c2_) : c1(c1_), c2(c2_) { }

	genome() { 
        c1.reset(new chrome());
        c2.reset(new chrome());
    }

    std::shared_ptr<chrome> mutate_x_over() {
        return c1->xover_mut(c2); 
    }

};

#endif
