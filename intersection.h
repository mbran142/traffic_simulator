#ifndef _INTERSECTION_H
#define _INTERSECTION_H

#include "road.h"
#include "signal.h"
#include <vector>

class Intersection {
public:
    Intersection();
    ~Intersection();
    void runSimulation(int);
    std::string createIntersectionString() const;
    friend std::ostream& operator<<(std::ostream& os, const Intersection& intersection);
    Grid& getGrid() const;
    void tick();
    Lane* getLane(int, int, int) const;
private:
    Grid* grid;
    Crossroad* crossroad;
    SignalSystem* signalSystem;
    static std::string codeChar(int[]);
};

#endif