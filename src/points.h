#include <iostream>

class points {
public:
    
    double x, y;
    points(double x_, double y_) : x(x_), y(y_) {}
    void setPoints(double x_, double y_) { x = x_; y = y_; }
    points() {};

    double distance(std::shared_ptr<points> p) {
        return pow(p->x - x, 2) + pow(p->y - y, 2); 
    }
} ;
