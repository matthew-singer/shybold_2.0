#ifndef LOOKUP_H_
#define LOOKUP_H_

#include "agents.h"
#include "params.h"

#include <vector>
#include <array>
#include <iostream>
#include <memory>
class Lookup {
    public:
    std::array < std::array<     std::vector< std::shared_ptr<agent> >  , breakupY + 1  >, breakupX + 1  >   lookupTable;
    
    template<std::size_t SIZE>
    Lookup( std::array<std::shared_ptr<agent>, SIZE> &lookup_agents) {
        for (auto &a: lookup_agents) {
            lookupTable[getLocY(a)][getLocX(a)].push_back(a);
        }
    }

    int getLocY(std::shared_ptr<agent> &a) {
        return int(a->y / (sizeY / breakupY));
    }

    int getLocX(std::shared_ptr<agent> &a) {
        return int(a->x / (sizeX / breakupX));
    }

    void valid_agent(double sensing_radius,  std::shared_ptr<agent> &base_agent) {
        bool search = true;
        int level = 0;
        int y_search = getLocY(base_agent);
        int x_search = getLocX(base_agent);
        base_agent->input_agent[0] = nullptr;
        int searchLast = 1; //some sort of exhaustive search

        while (level < breakupX && level < breakupY && searchLast) {
            if (base_agent->input_agent[0]) {
                searchLast = 0;
            }
            for (int x = std::max(x_search - level, 0) ; x <= std::min(x_search + level, breakupX); ++x) {
                for (int y = std::max(y_search - level, 0) ; y <= std::min(y_search + level, breakupY) ; ++y) {
                    if ( (x == x_search - level || x == x_search + level || y == y_search + level || y == y_search - level)  ) {
                        for (auto &a : lookupTable[y][x]) {
                            if (base_agent->valid_agent(a)) {
                                base_agent->input_agent[0] = a;
                            }
                        }
                    }

                }
            }
            ++level;
        }

    }


    void valid_agent( std::shared_ptr<agent> &base_agent) {
        bool search = true;
        int level = 0;
        int y_search = getLocY(base_agent);
        int x_search = getLocX(base_agent);
        std::shared_ptr<agent> tmp_a = nullptr;
        int searchLast = 1; //some sort of exhaustive search

        while (level < breakupX && level < breakupY && searchLast) {
            if (tmp_a) {
                searchLast = 0;
            }
            for (int x = std::max(x_search - level, 0) ; x <= std::min(x_search + level, breakupX); ++x) {
                for (int y = std::max(y_search - level, 0) ; y <= std::min(y_search + level, breakupY) ; ++y) {
                    if ( (x == x_search - level || x == x_search + level || y == y_search + level || y == y_search - level)  ) {
                        for (auto &a : lookupTable[y][x]) {
                            if (base_agent->valid_agent(a)) {
                                base_agent->input_agent[0] = a;
                            }
                        }
                    }

                }
            }
            ++level;
        }
    }

    void update( std::shared_ptr<agent> &a, int oldX, int oldY) {
        lookupTable[oldY][oldX].erase(std::remove(lookupTable[oldY][oldX].begin(), lookupTable[oldY][oldX].end(), a), lookupTable[oldY][oldX].end());
        lookupTable[getLocY(a)][getLocX(a)].push_back(a);
    }

};

#endif
