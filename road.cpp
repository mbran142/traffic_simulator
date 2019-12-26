#include "road.h"
#include "vehicle.h"

extern int totalTicks;

//[Spawn vs End][N E S W][Left Middle Right][s_x s_y e_x e_y]
const int Road::LANE_LOC[IN_OUT_COUNT][NUM_ROADS][NUM_LANES][START_END_COUNT] =
{
    { /* spawn lanes */
        { { 31, 61, 31, 36 }, { 32, 61, 32, 36 }, { 33, 61, 33, 36 } }, //north
        { { 00, 31, 25, 31 }, { 00, 32, 25, 32 }, { 00, 33, 25, 33 } }, //east
        { { 30, 00, 30, 25 }, { 29, 00, 29, 25 }, { 28, 00, 28, 25 } }, //south
        { { 61, 30, 36, 30 }, { 61, 29, 36, 29 }, { 61, 28, 36, 28 } }  //west
    },
    { /* end lanes */
        { { 31, 25, 31, 00 }, { 32, 25, 32, 00 }, { 33, 25, 33, 00 } }, //north
        { { 36, 31, 61, 31 }, { 36, 32, 31, 32 }, { 36, 33, 61, 33 } }, //east
        { { 30, 36, 30, 61 }, { 29, 36, 29, 61 }, { 28, 36, 28, 61 } }, //south
        { { 25, 30, 00, 30 }, { 25, 29, 00, 29 }, { 25, 28, 00, 28 } }  //west
    }
};

//Constructor for grid
Grid::Grid() {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            grid[i][j] = EMPTY;
}

//Checks if this gridspace is open
bool Grid::isOpen(int i, int j) const {
    return grid[i][j] == -1;
}

//Decides which lines to draw. Returns in a 2-digit decimal value of vh
Gridpoint Grid::drawRoadLine(int i, int j) {
    return Gridpoint(Grid::lineStatus(i, j), Grid::lineStatus(j, i));
}

int Grid::lineStatus(int i, int j) {
    int status = EMPTY;
    if (binarySearch(j, (const int[]){26, 27, 34, 35}, 4) && (i == 25 || i == 35))
        status = BOTH_LINES;
    else if ((j > 0 && j < 26) || (j > 35 && j < 62)) {
        if (binarySearch(i, (const int[]){27, 30, 33}, 3))
            status = BOTH_LINES;
        else if (binarySearch(i, (const int[]){28, 29, 31, 32}, 4))
            status = j < 26 ? BOTTOM_LINE : TOP_LINE;
    }
    return status;
}

//Creates a lane on a road
Lane::Lane(int dir, Gridpoint start, Gridpoint end, const Intersection* itref) : DIRECTION(dir), START(start), END(end) {

    this->itref = itref;

    //lane starts empty (NULL => empty space)
    for (int i = 0; i < LANE_SIZE; i++)
        space[i] = NULL;
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

//Determines the point at which vehicles spawn in a lane
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

//decide whether to spawn a vehicle based on rng and time
bool Spawnlane::decideToSpawnVehicle() {

    /*
                [226-299] is low
    [0,100] and [176-225] is medium
                [101-175] is high
    */

    int chance, curTime = totalTicks % 300;

    if (curTime > 225)
        chance = low_rate;
    else if (curTime > 100 && curTime <= 175)
        chance = high_rate;
    else
        chance = med_rate;
    
    return (rand() % 100 + 1) <= chance;
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

    if (Spawnlane::decideToSpawnVehicle())
        this->spawnVehicle();

    for (int i = 0; i < LANE_SIZE; i++) {

        //tick each vehicle
        if (space[i] != NULL) {
            space[i]->tick();
            i += space[i]->getSize() - 1;
        }
    }

    //update the position of all cars; if cars have left the lane, then give them to the intersection or whatever
    //FIGURE OUT HOW TO DO THIS NEXT^
}

//Adds a new vehicle to the queue
void Spawnlane::spawnVehicle() {
    vehicleQueue->push(Vehicle::generateRandomVehicle(SPAWNPOINT, itref));
}

//Default constructor for endlane
Endlane::Endlane(int dir, Gridpoint start, Gridpoint end, const Intersection* itref) : Lane(dir, start, end, itref), ENDPOINT(Endlane::determineEndpoint(start, end)) { }

//Default destructor for endlane
Endlane::~Endlane() { }

//Determines the despawn point for cars on an endlane
Gridpoint Endlane::determineEndpoint(Gridpoint start, Gridpoint end) {
    return end;
    //TODO: consider changing this
}

//Simulates one unit of time
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

//Destructor for road
Road::~Road() {
    for (int i = 0; i < NUM_LANES_PER_ROAD; i++)
        delete lane[i];
}

//Ticks the road one unit further in time
void Road::tick() {
    for (int i = 0; i < NUM_LANES; i++)
        lane[i]->tick();
}

//Constructor for crossroad
Crossroad::Crossroad(const Intersection* itref) {
    for (int i = 0; i < NUM_ROADS; i++) {
        inRoad[i] = new Road(i, true, itref);
        outRoad[i] = new Road(i, false, itref);
    }
}

//Destructor for crossroad
Crossroad::~Crossroad() {
    for (int i = 0; i < NUM_ROADS; i++) {
        delete inRoad[i];
        delete outRoad[i];
    }
}

//Move the elements of the crossroad one unit in time
void Crossroad::tick() {
    for (int i = 0; i < NUM_ROADS; i++) {
        inRoad[i]->tick();
        outRoad[i]->tick();
    }
}