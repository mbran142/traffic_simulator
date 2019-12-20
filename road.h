#ifndef _ROAD_H
#define _ROAD_H

#include "traffic.h"

class Grid {
public:
    Grid();
    bool isOpen(int, int);
private:
    bool grid[GRID_SIZE][GRID_SIZE];
};

class Lane {
public:
    Lane(int, Gridpoint, Gridpoint, bool);
    bool backIsOpen() const;
    int backSpacesOpen() const;
    int getDirection() const;
private:
    const bool SPAWNER;
    const int DIRECTION;
    Gridpoint specialPoint; //spawnpoint or killpoint
};

class Road {
public:
    Road(int);
    ~Road();
private:
    Lane* lane[NUM_LANES_PER_ROAD];
    const int DIRECTION;
};

class Crossroad {
public:
    Crossroad();
    ~Crossroad();
private:
    Road* inRoad[NUM_ROADS];
    Road* outRoad[NUM_ROADS];
};

#endif