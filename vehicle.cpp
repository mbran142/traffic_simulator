#include "vehicle.h"
#include "intersection.h"
#include "road.h"

//declaring table of vehicle accelerations, max speeds, and sizes
const int Vehicle::VCONST[SIZE_VEHICLES][SIZE_CONSTS] = { { C_ACC, C_SPD, C_SIZE },
                                                          { V_ACC, V_SPD, V_SIZE },
                                                          { M_ACC, M_SPD, M_SIZE },
                                                          { T_ACC, T_SPD, T_SIZE } };

//Vehicle constructor
Vehicle::Vehicle(const Gridpoint& gp, const Intersection* itref, const int consts[SIZE_CONSTS]) :
ACCELERATION_MAX(consts[ACCELERATION]), SPEED_MAX(consts[SPEED]), VEHICLE_SIZE(consts[SIZE]), position(gp) {

    this->itref = itref;

    //find the lane this Vehicle was spawned in
    for (int i = 0; i < NUM_ROADS; i++)
        for (int j = 0; j < NUM_LANES; j++)
            if (gp.x == Road::LANE_LOC[IN_LANE][i][j][START_X] && gp.y == Road::LANE_LOC[IN_LANE][i][j][START_Y])
                curLane = itref->getLane(IN_LANE, j, i);

    //Set up positions and such
    distanceLeftInLane = curLane->getSize();
    buttLane = curLane;
    position = curLane->getStart();
    buttPosition = position;
    buttPosition.move(curLane->getDirection(), VEHICLE_SIZE - 1, false);
    buttInLane = 0;

    //calculate initial speed
    acceleration = 0;
    speed = SPEED_MAX;
    while (this->tooFast())
        speed--;

    //decide which direction to turn
    {
        int rng = rand() % 100;
        switch (curLane->getPosition()) {
            case LEFT  : destination = rng < u_turn_rate ? U_TURN : LEFT_TURN; break;
            case MIDDLE: destination = STRAIGHT; break;
            case RIGHT : destination = rng < right_turn_rate ? RIGHT_TURN : STRAIGHT; break;
        }
    }
};

//Simulates one unit of time for the vehicle
void Vehicle::tick() {

    int temp, toTravel;

    //change acceleration
    temp = speed;
    acceleration = ACCELERATION_MAX + 1;
    do {
        acceleration--;
        speed = temp;
        speed += acceleration;
    } while (acceleration >= DECELERATION * -1 && this->tooFast());
    
    //change speed;
    speed = temp + acceleration;

    if (speed > distanceLeftInLane) {
        toTravel = speed - distanceLeftInLane + 1;
        buttInLane = distanceLeftInLane + SIZE - speed;
        curLane = curLane->getNextLane(destination);
        position = curLane->getStart();
        distanceLeftInLane = curLane->getSize();
    }
    else toTravel = speed;

    //change position
    switch (curLane->getDirection()) {
        case NORTH: position.y += toTravel; break;
        case EAST : position.x += toTravel; break;
        case SOUTH: position.y -= toTravel; break;
        case WEST : position.x -= toTravel; break;
        case NORTHEAST: position.y += toTravel; position.x += toTravel; break;
        case SOUTHEAST: position.y -= toTravel; position.x += toTravel; break;
        case SOUTHWEST: position.y -= toTravel; position.x -= toTravel; break;
        case NORTHWEST: position.y += toTravel; position.x -= toTravel; break;
    }
    distanceLeftInLane -= toTravel;

    //update grid
    Gridpoint gridUpdator = position;
    for (int i = 0; i < speed; i++) {

        //modify front and back of grid
        itref->getGrid().setSpace(gridUpdator, true);
        itref->getGrid().setSpace(buttPosition, false);

        //update positions
        gridUpdator.move(curLane->getDirection(), 1, false);
        buttPosition.move(buttLane->getDirection(), 1, true);
        if (buttInLane) {
            buttInLane--;
            if (!buttInLane)
                buttLane = curLane;
        }
    }

    /*
    STUFF NEXT
    - implement Vehicle::tooFast()
        + decide how Vehicle will be able to check cars in front of it and red lights and stuff
    */
}

int Vehicle::getSize() const {
    return VEHICLE_SIZE;
}

//creates a random vehicle
Vehicle* Vehicle::generateRandomVehicle(Gridpoint location, const Intersection* itref) {
    
    int rng = rand() % 100 + 1;
    int threshhold = 0;

    //car
    threshhold += car_rate;
    if (rng <= threshhold)
        return new Car(location, itref);
    
    //van
    threshhold += van_rate;
    if (rng <= van_rate)
        return new Van(location, itref);

    //motercycle
    threshhold += motercycle_rate;
    if (rng <= motercycle_rate)
        return new Motercycle(location, itref);
        
    //truck
    else return new Truck(location, itref);
}

//checks the traffic signal in front of the car
bool Vehicle::checkSignal() const {
    //this should check if the signal in front of them allows them to go through the intersection
    //green = go, red = stop, yellow = calculate it!
    //true means go, false means stop
    return true;
}

//checks if the vehicle is on track to run a red light
bool Vehicle::goingToRunRedLight() const {
    //check if this guys gonna run a red light. take into account yellows!
    return false;
}

//checks if the vehicle is on track to read-end someone
bool Vehicle::goingToRearEnd() const {
    //check if this guys gonna read end the guy in front of him. Take into account their speed
    //cars should be at least [NEXT_CAR_SPEED] spaces away from the car in front of them
    return false;
}

//Returns true if this vehicle is safe to continue at its speed
bool Vehicle::tooFast() const {
    
    if (this->goingToRunRedLight() || this->goingToRearEnd())
        return true;

    //TODO: add more
    return false;
}

//Creates a car
Car::Car(const Gridpoint& gp, const Intersection* itref) : Vehicle(gp, itref, Vehicle::VCONST[CAR]) {};

//Creates a van
Van::Van(const Gridpoint& gp, const Intersection* itref) : Vehicle(gp, itref, Vehicle::VCONST[VAN]) {};

//Creates a motercycle
Motercycle::Motercycle(const Gridpoint& gp, const Intersection* itref) : Vehicle(gp, itref, Vehicle::VCONST[MOTERCYCLE]) {};

//Creates a 12-wheeler truck
Truck::Truck(const Gridpoint& gp, const Intersection* itref) : Vehicle(gp, itref, Vehicle::VCONST[TRUCK]) {};