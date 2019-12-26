#ifndef _INTERSECTION_H
#define _INTERSECTION_H

#include "road.h"
#include "signal.h"
#include <vector>

class Intersection {
public:
    Intersection();
    ~Intersection();
    void runSimulation();
    friend std::ostream& operator<<(std::ostream& os, const Intersection& intersection);
    Grid& getGrid() const;
    void tick();
private:
    Grid* grid;
    Crossroad* crossroad;
    SignalSystem* signalSystem;
    static char codeChar(int);
};

#endif