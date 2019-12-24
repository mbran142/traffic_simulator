#include "road.h"
#include "vehicle.h"

//[Spawn vs End][N E S W][Left Middle Right][s_x s_y e_x e_y]
const int Road::LANE_LOC[IN_OUT_COUNT][NUM_ROADS][NUM_LANES][START_END_COUNT] =
    {
    /* spawn lanes */ {
        { { 31, 61, 31, 36 }, { 32, 61, 32, 36 }, { 33, 61, 33, 36 } }, //north
        { { 00, 31, 25, 31 }, { 00, 32, 25, 32 }, { 00, 33, 25, 33 } }, //east
        { { 30, 00, 30, 25 }, { 29, 00, 29, 25 }, { 28, 00, 28, 25 } }, //south
        { { 61, 30, 36, 30 }, { 61, 29, 36, 29 }, { 61, 28, 36, 28 } }  //west
    },
    /* end lanes */ {
        { { 31, 25, 31, 00 }, { 32, 25, 32, 00 }, { 33, 25, 33, 00 } }, //north
        { { 36, 31, 61, 31 }, { 36, 32, 31, 32 }, { 36, 33, 61, 33 } }, //east
        { { 30, 36, 30, 61 }, { 29, 36, 29, 61 }, { 28, 36, 28, 61 } }, //south
        { { 25, 30, 00, 30 }, { 25, 29, 00, 29 }, { 25, 28, 00, 28 } }  //west
    }
    };

//
Grid::Grid() {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            grid[i][j] = false;
}

//
bool Grid::isOpen(int i, int j) {
    return grid[i][j];
}


//Creates a lane on a road
Lane::Lane(int dir, Gridpoint start, Gridpoint end, const Intersection* itref) : DIRECTION(dir), START(start), END(end) {
    intersection = itref;
}

//Default lane destructor
Lane::~Lane() { }

//Gets the direction the lane is going
int Lane::getDirection() const {
    return DIRECTION;
}

//Create new spawnlane
Spawnlane::Spawnlane(int dir, Gridpoint start, Gridpoint end, const Intersection* itref) : Lane(dir, start, end, itref), SPAWNPOINT(Spawnlane::determineSpawnpoint(start, end)) {
    vehicleQueue = new std::queue<Vehicle*>();
}

//Deallocate resources used by spawnlane
Spawnlane::~Spawnlane() {

    //free contents of queue
    while (!vehicleQueue->empty()) {
        delete vehicleQueue->front();
        vehicleQueue->pop();
    }

    //free queue
    delete vehicleQueue;
}

//
Gridpoint Spawnlane::determineSpawnpoint(Gridpoint start, Gridpoint end) {

    int x, y;

    if (start.x == end.x) {
        x = start.x;
        y = start.y > end.y ? start.y + T_SIZE : start.y - T_SIZE;
    }
    else {
        y = start.y;
        x = start.x + (start.x > end.y ? T_SIZE : T_SIZE * -1);
    }

    return Gridpoint(x, y);
}

//Checks if there is space for a car to be removed from the queue
bool Spawnlane::backIsOpen() const {
    //check if there is room for another car to be spawned if there's one in the spawn queue
    return true;
}

//Checks how many spaces are open in front of a newly spawned car
int Spawnlane::backSpacesOpen() const {
    //check how many spaces are open in the back of the lane
    return 0;
}

//Ticks the road one unit in time
void Spawnlane::tick() {
    //TODO: implement this; it should tick every vehicle in the lane
}

//Adds a new vehicle to the queue
void Spawnlane::spawnVehicle() {
    vehicleQueue->push(Vehicle::generateRandomVehicle(SPAWNPOINT, intersection));
}

//Default constructor for endlane
Endlane::Endlane(int dir, Gridpoint start, Gridpoint end, const Intersection* itref) : Lane(dir, start, end, itref), ENDPOINT(Endlane::determineEndpoint(start, end)) { }

//Default destructor for endlane
Endlane::~Endlane() { }

//
Gridpoint Endlane::determineEndpoint(Gridpoint start, Gridpoint end) {
    return end;
    //TODO: consider changing this
}

void Endlane::tick() {
    //TODO: implement this; should destroy vehicles that have reached the endpoint
}

//
Road::Road(int dir, bool in, const Intersection* itref) : DIRECTION(dir) {

    for (int i = 0; i < NUM_LANES_PER_ROAD; i++) {
        if (in) lane[i] = new Spawnlane(dir,
                Gridpoint(Road::LANE_LOC[IN_LANE][dir][i][START_X], Road::LANE_LOC[IN_LANE][dir][i][START_Y]),
                Gridpoint(Road::LANE_LOC[IN_LANE][dir][i][END_X], Road::LANE_LOC[IN_LANE][dir][i][END_Y]), itref);
        else    lane[i] = new Endlane(dir,
                Gridpoint(Road::LANE_LOC[OUT_LANE][dir][i][START_X], Road::LANE_LOC[OUT_LANE][dir][i][START_Y]),
                Gridpoint(Road::LANE_LOC[OUT_LANE][dir][i][END_X], Road::LANE_LOC[OUT_LANE][dir][i][END_Y]), itref);
    }
}

//
Road::~Road() {
    for (int i = 0; i < NUM_LANES_PER_ROAD; i++)
        delete lane[i];
}


//
Crossroad::Crossroad(const Intersection* itref) {
    for (int i = 0; i < NUM_ROADS; i++) {
        inRoad[i] = new Road(i, true, itref);
        outRoad[i] = new Road(i, false, itref);
    }
}

//
Crossroad::~Crossroad() {
    for (int i = 0; i < NUM_ROADS; i++) {
        delete inRoad[i];
        delete outRoad[i];
    }
}