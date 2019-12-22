#include "road.h"
#include "vehicle.h"

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

//
Gridpoint Endlane::determineEndpoint(Gridpoint start, Gridpoint end) {
    return end;
    //TODO: consider changing this
}

//
Road::Road(int direction, bool in) : DIRECTION(direction) {

    for (int i = 0; i < NUM_LANES_PER_ROAD; i++) {
        //TODO: change this
        //if (in) lane[i] = new Spawnlane(direction, );
        //else    lane[i] = new Endlane(direction,);
        std::cout << "test" << std::endl;
    }
}

//
Road::~Road() {
    for (int i = 0; i < NUM_LANES_PER_ROAD; i++) {
        delete lane[i];
    }
}


//
Crossroad::Crossroad() {
    for (int i = 0; i < NUM_ROADS; i++) {
        inRoad[i] = new Road(i, true);
        outRoad[i] = new Road(i, false);
    }
}

//
Crossroad::~Crossroad() {
    for (int i = 0; i < NUM_ROADS; i++) {
        delete inRoad[i];
        delete outRoad[i];
    }
}