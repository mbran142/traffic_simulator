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


//Creates a lane on a road
Lane::Lane(int dir, Gridpoint startpoint, Gridpoint endpoint, bool b) : SPAWNER(b), DIRECTION(dir) {
    
    //TODO: SPLIT THIS UP INTO SPAWNLANE AND ENDLANES ***REMEMBER THAT THE KILL TILES FOR A KILL LANE SHOULD = MAX SPEED
    //decide where the special point is
    if (SPAWNER) {

        int x, y;

        if (startpoint.x == endpoint.x) {
            x = startpoint.x;
            y = startpoint.y > endpoint.y ? startpoint.y + T_SIZE : startpoint.y - T_SIZE;
        }
        else {
            y = startpoint.y;
            x = startpoint.x + (startpoint.x > endpoint.y ? T_SIZE : T_SIZE * -1);
        }

        specialPoint.x = x;
        specialPoint.y = y;
    }

    else specialPoint = endpoint;
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

//Gets the direction the lane is going
int Lane::getDirection() const {
    return DIRECTION;
}


//
Road::Road(int direction) : DIRECTION(direction) {
    for (int i = 0; i < NUM_LANES_PER_ROAD; i++) {
        //TODO: change this
        lane[i] = new Lane(direction, Gridpoint(0,0), Gridpoint(0,0), true);
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
        inRoad[i] = new Road(i);
        outRoad[i] = new Road(i);
    }
}

//
Crossroad::~Crossroad() {
    for (int i = 0; i < NUM_ROADS; i++) {
        delete inRoad[i];
        delete outRoad[i];
    }
}