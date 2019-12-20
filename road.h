#ifndef _ROAD_H
#define _ROAD_H

#include "traffic.h"

struct Gridpoint {
    int x;
    int y;
    Gridpoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Grid {
public:
    Grid();
    bool isOpen(int, int);
private:
    bool grid[GRID_SIZE][GRID_SIZE];
};

class Lane {
public:
    Lane(Gridpoint startpoint, Gridpoint endpoint, bool spawnLane);
    bool backIsOpen() const;
    int backSpacesOpen() const;
    Gridpoint getSpawnPoint() const;
    int getDirection() const;
private:
    const int DIRECTION;
};

class Road {
public:
    Road();
    ~Road();
private:
    Lane* lane[NUM_LANES_PER_ROAD];
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