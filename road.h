#ifndef _ROAD_H
#define _ROAD_H

#include "traffic.h"
#include <queue>

class Vehicle;
class Intersection;

class Grid {
public:
    Grid();
    bool isOpen(int, int) const;
    static Gridpoint drawRoadLine(int, int);
private:
    int grid[GRID_SIZE][GRID_SIZE];
    static int lineStatus(int, int);
};

class Lane {
public:
    Lane(int, const Gridpoint&, const Gridpoint&, const Intersection*);
    virtual ~Lane();
    int getDirection() const;
    virtual void tick() = 0;
    static int determineLaneSize(int, const Gridpoint&, const Gridpoint&);
    void connectLane(const Lane*, int);
protected:
    const int DIRECTION, THIS_LANE_SIZE;
    const Gridpoint START, END;
    const Intersection* itref;
    const Lane* connection[POSSIBLE_TURNS];
    Vehicle** space;
};

class Spawnlane : public Lane {
public:
    Spawnlane(int, const Gridpoint&, const Gridpoint&, const Intersection*);
    ~Spawnlane();
    static Gridpoint determineSpawnpoint(const Gridpoint&, const Gridpoint&);
    static bool decideToSpawnVehicle();
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
    Endlane(int, const Gridpoint&, const Gridpoint&, const Intersection*);
    ~Endlane();
    static Gridpoint determineEndpoint(const Gridpoint&, const Gridpoint&);
    void tick();
private:
    const Gridpoint ENDPOINT;
};

class IntersectionLane : public Lane {
public:
    IntersectionLane(int, const Gridpoint&, const Gridpoint&, const Intersection*);
    ~IntersectionLane();
    void tick();
private:
};

class Road {
public:
    const static int LANE_LOC[IN_OUT_COUNT][NUM_ROADS][NUM_LANES][START_END_COUNT];
    const static int INTERSECTION_LANE_LOC[INTERSECTION_LANE_SIZE][NUM_ROADS][START_END_COUNT];
    Road(int, bool, const Intersection*);
    ~Road();
    void tick();
    Lane* getLane(int) const;
private:
    Lane* lane[NUM_LANES_PER_ROAD];
    const int DIRECTION;
};

class Crossroad {
public:
    Crossroad(const Intersection*);
    ~Crossroad();
    void tick();
    Lane* getLane(int, int, int) const;
private:
    Road* inRoad[NUM_ROADS];
    Lane* interLane[NUM_ROADS][INTERSECTION_LANE_SIZE]; //Ne Se Sw Ne OR N E S W -> left, right, straight_L, straight_R
    Road* outRoad[NUM_ROADS];
};

#endif