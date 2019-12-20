#ifndef _ROAD_H
#define _ROAD_H

#include "traffic.h"
#include <vector>

class Lane {
public:

private:
    
};

class Road {
public:

private:
    Lane lanes[NUM_LANES_PER_ROAD];
};

class Crossroad {
public:

private:
    Road roads[NUM_ROADS];
};

#endif