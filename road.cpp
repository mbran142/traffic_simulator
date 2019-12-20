#include "road.h"

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


//
Lane::Lane(Gridpoint startpoint, Gridpoint endpoint, bool spawnpoint) {
    //create a lane using this stuff
}

//
bool Lane::backIsOpen() const {
    //check if there is room for another car to be spawned if there's one in the spawn queue
    return true;
}

//
int Lane::backSpacesOpen() const {
    //check how many spaces are open in the back of the lane
    return 0;
}

//
Gridpoint Lane::getSpawnPoint() const {
    //return the spawn point of this lane (NOT THE START; THE SPAWN)
    return Gridpoint(0,0);
}

//
int Lane::getDirection() const {
    return DIRECTION;
}


//
Road::Road() {
    for (int i = 0; i < NUM_LANES_PER_ROAD; i++) {
        //TODO: change this
        lane[i] = new Lane(Gridpoint(0,0), Gridpoint(0,0), true);
    }
}

//
Road::~Road() {
    for (int i = 0; i < NUM_LANES_PER_ROAD; i++) {
        delete lane[i];
    }
}


//IMPLEMENT CROSSROAD STUFF