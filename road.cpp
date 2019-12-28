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
        { { 36, 31, 61, 31 }, { 36, 32, 61, 32 }, { 36, 33, 61, 33 } }, //east
        { { 30, 36, 30, 61 }, { 29, 36, 29, 61 }, { 28, 36, 28, 61 } }, //south
        { { 25, 30, 00, 30 }, { 25, 29, 00, 29 }, { 25, 28, 00, 28 } }  //west
    }
};

//[Left vs Right turn][N E W S][s_x s_y e_x e_y]
const int Road::INTERSECTION_LANE_LOC[INTERSECTION_LANE_SIZE][NUM_ROADS][START_END_COUNT] =
{
    { /* left turn */
        { 26, 31, 31, 26 }, //northeast
        { 30, 26, 35, 31 }, //southeast
        { 35, 30, 30, 35 }, //southwest
        { 31, 35, 26, 30 }  //northwest
    },
    { /* right turn */
        { 33, 35, 35, 33 }, //northeast
        { 26, 33, 28, 35 }, //southeast
        { 28, 26, 26, 28 }, //southwest
        { 35, 28, 33, 26 }   //northwest
    },
    { /* left straight */
        { 32, 35, 32, 26 }, //north
        { 26, 32, 35, 32 }, //east
        { 29, 26, 29, 35 }, //south
        { 35, 29, 26, 29 }  //west
    },
    { /* right straight */
        { 33, 35, 33, 26 }, //north
        { 26, 33, 35, 33 }, //east
        { 28, 26, 28, 35 }, //south
        { 35, 28, 26, 28 }  //west
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
Lane::Lane(int dir, const Gridpoint& start, const Gridpoint& end, const Intersection* itref) : DIRECTION(dir), THIS_LANE_SIZE(Lane::determineLaneSize(dir, start, end)), START(start), END(end) {

    this->itref = itref;
    space = new Vehicle*[THIS_LANE_SIZE];

    //lane starts empty (NULL => empty space)
    for (int i = 0; i < THIS_LANE_SIZE; i++)
        space[i] = NULL;

    for (int i = 0; i < POSSIBLE_TURNS; i++)
        connection[i] = NULL;
}

//Default lane destructor
Lane::~Lane() {
    delete [] space;
}

//Gets the direction the lane is going
int Lane::getDirection() const {
    return DIRECTION;
}

//Determines lane size
int Lane::determineLaneSize(int dir, const Gridpoint& s, const Gridpoint& e) {

    switch (dir) {
        case NORTH:     return s.y - e.y;
        case EAST:      return e.x - s.x;
        case SOUTH:     return e.y - s.y;
        case WEST:      return s.x - e.x;
        case NORTHEAST: return e.x - s.x;
        case SOUTHEAST: return e.x - s.x;
        case SOUTHWEST: return s.x - e.x;
        case NORTHWEST: return s.x - e.x;
        default: throw "Error: Lane::determineLaneSize()";
    }

    //to satisfy compiler
    return 0;
}

//Connects one lane to this one
void Lane::connectLane(const Lane* toConnect, int turnDirection) {
    connection[turnDirection] = toConnect;
}

//Create new spawnlane
Spawnlane::Spawnlane(int dir, const Gridpoint& start, const Gridpoint& end, const Intersection* itref) : Lane(dir, start, end, itref), SPAWNPOINT(Spawnlane::determineSpawnpoint(start, end)) {
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
Gridpoint Spawnlane::determineSpawnpoint(const Gridpoint& start, const Gridpoint& end) {

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

    for (int i = THIS_LANE_SIZE - 1; i >= 0; --i) {
        //tick each vehicle
        if (space[i] != NULL) {
            space[i]->tick();
            i -= space[i]->getSize() - 1;
        }
    }
}

//Adds a new vehicle to the queue
void Spawnlane::spawnVehicle() {
    vehicleQueue->push(Vehicle::generateRandomVehicle(SPAWNPOINT, itref));
}

//Default constructor for endlane
Endlane::Endlane(int dir, const Gridpoint& start, const Gridpoint& end, const Intersection* itref) : Lane(dir, start, end, itref), ENDPOINT(Endlane::determineEndpoint(start, end)) { }

//Default destructor for endlane
Endlane::~Endlane() { }

//Determines the despawn point for cars on an endlane
Gridpoint Endlane::determineEndpoint(const Gridpoint& start, const Gridpoint& end) {
    return end;
    //TODO: consider changing this
}

//Simulates one unit of time
void Endlane::tick() {

}

//Turnlane constructor
IntersectionLane::IntersectionLane(int dir, const Gridpoint& start, const Gridpoint& end, const Intersection* itref) : Lane(dir, start, end, itref) { }

//Turnlane destructor
IntersectionLane::~IntersectionLane() { }

void IntersectionLane::tick() {

    for (int i = THIS_LANE_SIZE - 1; i >= 0; --i) {
        //tick each vehicle
        if (space[i] != NULL) {
            space[i]->tick();
            i -= space[i]->getSize() - 1;
        }
    }
}

//Road constructor
Road::Road(int dir, bool in, const Intersection* itref) : DIRECTION(dir) {

    for (int i = 0; i < NUM_LANES_PER_ROAD; i++) {
        if (in)
            lane[i] = new Spawnlane(dir, Gridpoint(Road::LANE_LOC[IN_LANE][dir][i][START_X], Road::LANE_LOC[IN_LANE][dir][i][START_Y]), Gridpoint(Road::LANE_LOC[IN_LANE][dir][i][END_X], Road::LANE_LOC[IN_LANE][dir][i][END_Y]), itref);
        else
            lane[i] = new Endlane(dir, Gridpoint(Road::LANE_LOC[OUT_LANE][dir][i][START_X], Road::LANE_LOC[OUT_LANE][dir][i][START_Y]), Gridpoint(Road::LANE_LOC[OUT_LANE][dir][i][END_X], Road::LANE_LOC[OUT_LANE][dir][i][END_Y]), itref);
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

//Gets a lane reference from a road
Lane* Road::getLane(int left_right_middle) const {
    return lane[left_right_middle];
}

//Constructor for crossroad
Crossroad::Crossroad(const Intersection* itref) {

    int direction;

    //instantiate lanes and roads
    for (int i = 0; i < NUM_ROADS; i++) {

        inRoad[i] = new Road(i, true, itref);
        outRoad[i] = new Road(i, false, itref);

        //first two are turn lanes => add 4 to direction to convert to diagonal
        for (int j = 0; j < INTERSECTION_LANE_SIZE; j++) {

            //THINK ABOUT THIS LINE 
            direction = j < 2 ? i + 4 : i;

            interLane[i][j] = new IntersectionLane(direction, Gridpoint(Road::INTERSECTION_LANE_LOC[j][i][START_X], Road::INTERSECTION_LANE_LOC[j][i][START_Y]), Gridpoint(Road::INTERSECTION_LANE_LOC[j][i][END_X], Road::INTERSECTION_LANE_LOC[j][i][END_Y]), itref);
        }
    }

    //connect lanes together
    for (int i = 0; i < NUM_ROADS; i++) {

        //in roads
        inRoad[i]->getLane(LEFT)->connectLane(interLane[(i + 3) % 4][CR_LEFT], LEFT_TURN);
        inRoad[i]->getLane(LEFT)->connectLane(outRoad[(i + 2) % 4]->getLane(LEFT), U_TURN);
        inRoad[i]->getLane(MIDDLE)->connectLane(interLane[i][CR_STRAIGHT_L], STRAIGHT);
        inRoad[i]->getLane(RIGHT)->connectLane(interLane[i][CR_STRAIGHT_R], STRAIGHT);
        inRoad[i]->getLane(RIGHT)->connectLane(interLane[i][CR_RIGHT], RIGHT_TURN);

        //sublanes in intersection
        interLane[i][CR_LEFT]->connectLane(outRoad[(i + 3) % 4]->getLane(LEFT), LEFT_TURN);
        interLane[i][CR_RIGHT]->connectLane(outRoad[(i + 1) % 4]->getLane(RIGHT), RIGHT_TURN);
        interLane[i][CR_STRAIGHT_L]->connectLane(outRoad[i]->getLane(MIDDLE), STRAIGHT);
        interLane[i][CR_STRAIGHT_R]->connectLane(outRoad[i]->getLane(RIGHT), STRAIGHT);
    }
}

//Destructor for crossroad
Crossroad::~Crossroad() {
    for (int i = 0; i < NUM_ROADS; i++) {
        delete inRoad[i];
        delete outRoad[i];
        for (int j = 0; j < INTERSECTION_LANE_SIZE; j++)
            delete interLane[i][j];
    }
}

//Move the elements of the crossroad one unit in time
void Crossroad::tick() {
    for (int i = 0; i < NUM_ROADS; i++) {
        outRoad[i]->tick();
        inRoad[i]->tick();
        for (int j = 0; j < INTERSECTION_LANE_SIZE; j++)
            interLane[i][j]->tick();
    }
}

//Gets a reference to a lane
Lane* Crossroad::getLane(int in_out_turn, int left_right_middle, int dir) const {
    switch (in_out_turn) {
        case IN_LANE: return inRoad[dir]->getLane(left_right_middle);
        case OUT_LANE: return outRoad[dir]->getLane(left_right_middle);
        default: {
            int index = dir;
            if (index >= 4)
                index -= 4;
            return interLane[left_right_middle][index];
        }
    }
}