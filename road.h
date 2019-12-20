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
    Gridpoint getSpawnPoint() const;
    int getDirection() const;
private:
    const int DIRECTION;
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
    const int BEFORE_SIGNAL = 0;
    const int AFTER_SIGNAL = 1;
    Road* roads[2][NUM_ROADS];
};

#endif