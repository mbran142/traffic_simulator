#ifndef _INTERSECTION_H
#define _INTERSECTION_H

#include "road.h"
#include "signal.h"

class Intersection {
public:
    void runSimulation();
private:
    Grid grid;
    Crossroad crossroad;
    SignalSystem signalSystem;
    friend std::ostream& operator<<(std::ostream& os, const Intersection& intersection);
};

#endif