#ifndef _ROAD_H
#define _ROAD_H

#include "traffic.h"
#include <queue>

class Vehicle;
class Intersection;

class Grid {
public:
    Grid();
    bool isOpen(int, int);
private:
    bool grid[GRID_SIZE][GRID_SIZE];
};

class Lane {
public:
    Lane(int, Gridpoint, Gridpoint, const Intersection*);
    virtual ~Lane() = default;
    int getDirection() const;
    virtual void tick() = 0;
protected:
    const int DIRECTION;
    const Gridpoint START, END;
    const Intersection* intersection;
};

class Spawnlane : public Lane {
public:
    Spawnlane(int, Gridpoint, Gridpoint, const Intersection*);
    ~Spawnlane();
    static Gridpoint determineSpawnpoint(Gridpoint, Gridpoint);
    bool backIsOpen() const;
    int backSpacesOpen() const;
    void tick();
private:
    const Gridpoint SPAWNPOINT;
    std::queue<Vehicle*>* vehicleQueue;
    void spawnVehicle();
};

class Endlane : public Lane {
public:
    Endlane(int dir, Gridpoint start, Gridpoint end, const Intersection* itref) : Lane(dir, start, end, itref), ENDPOINT(Endlane::determineEndpoint(start, end)) { }
    ~Endlane() = default;
    static Gridpoint determineEndpoint(Gridpoint, Gridpoint);
    void tick();
private:
    const Gridpoint ENDPOINT;
};

class Road {
public:
    Road(int, bool);
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