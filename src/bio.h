#ifndef BIO_H_
#define BIO_H_

#include <vector>
#include <array>
#include <memory>

class chrome {
public:
    chrome(std::unique_ptr<chrome> base) {
        /*for (int i = 0; i < values.size(); ++i) {
            values[i] = (base->values)[i] + get_mutation();
        }*/
	}	

    chrome() {
        /*for (int i = 0; i < values.size(); ++i) {
           values[i] = normal_dist();
        }*/
    } 

	//std::array<double, 10> value;


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

};

#endif
