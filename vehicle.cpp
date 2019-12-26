#include "vehicle.h"

//declaring table of vehicle accelerations, max speeds, and sizes
const int Vehicle::VCONST[SIZE_VEHICLES][SIZE_CONSTS] = { { C_ACC, C_SPD, C_SIZE },
                                                          { V_ACC, V_SPD, V_SIZE },
                                                          { M_ACC, M_SPD, M_SIZE },
                                                          { T_ACC, T_SPD, T_SIZE } };

//COMMENT THIS
Vehicle::Vehicle(const Gridpoint& gp, const Intersection* itref, const int consts[SIZE_CONSTS]) :
ACCELERATION_MAX(consts[ACCELERATION]), SPEED_MAX(consts[SPEED]), VEHICLE_SIZE(consts[SIZE]), position(gp) {

    localEnvironment = itref;

    //calculate initial speed
    acceleration = 0;
    speed = SPEED_MAX;
    while (this->tooFast())
        speed--;

    if (gp.x == 0)
        direction = EAST;
    else if (gp.x == 61)
        direction = WEST;
    else if (gp.y == 0)
        direction = SOUTH;
    else
        direction = NORTH;

    destination = STRAIGHT; //REMOVE THIS LATER
    //TO DO: DECIDE DESTINATION (left lane = turn or U-turn, mid = straight, right = straight or right turn)
    //maybe decide this in the constructor, maybe not
};

//Simulates one unit of time for the vehicle
void Vehicle::tick() {

    int temp;

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

    //change position
    switch (direction) {
        case NORTH: position.y += speed; break;
        case EAST : position.x += speed; break;
        case SOUTH: position.y -= speed; break;
        case WEST : position.x -= speed; break;
        //TODO:
        // 1) if this vehicle passes over the bounds of its lane, find how far over it passed and send it somewhere else
    }

    /*
    STUFF NEXT
    - implement Vehicle::tooFast()
        + decide how Vehicle will be able to check cars in front of it and red lights and stuff
    - figure out how vehicles will go from lanes to the intersection to lanes
        + I'll probably add NE, NW, SE, SW directions and itll go diagonally through the intersection
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
    return !this->goingToRunRedLight() && !this->goingToRearEnd();
}

//Creates a car
Car::Car(const Gridpoint& gp, const Intersection* itref) : Vehicle(gp, itref, Vehicle::VCONST[CAR]) {};

//Creates a van
Van::Van(const Gridpoint& gp, const Intersection* itref) : Vehicle(gp, itref, Vehicle::VCONST[VAN]) {};

//Creates a motercycle
Motercycle::Motercycle(const Gridpoint& gp, const Intersection* itref) : Vehicle(gp, itref, Vehicle::VCONST[MOTERCYCLE]) {};

//Creates a 12-wheeler truck
Truck::Truck(const Gridpoint& gp, const Intersection* itref) : Vehicle(gp, itref, Vehicle::VCONST[TRUCK]) {};