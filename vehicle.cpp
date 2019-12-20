#include "vehicle.h"

//declaring table of vehicle accelerations, max speeds, and sizes
const int Vehicle::VCONST[SIZE_VEHICLES][SIZE_CONSTS] = { { C_ACC, C_SPD, C_SIZE },
                                                          { V_ACC, V_SPD, V_SIZE },
                                                          { M_ACC, M_SPD, M_SIZE },
                                                          { T_ACC, T_SPD, T_SIZE } };

//COMMENT THIS
Vehicle::Vehicle(const Gridpoint& gp, int road, int lane, const int consts[SIZE_CONSTS]) :
ACCELERATION_MAX(consts[ACCELERATION]), SPEED_MAX(consts[SPEED]), VEHICLE_SIZE(consts[SIZE]), position(gp) {

    localEnvironment = NULL;

    this->road = road;
    this->lane = lane;

    //calculate initial speed
    speed = SPEED_MAX;
    while (this->tooFast())
        speed--;

    //TODO: decide whether to tick() or not
};

//Creates a car
Car::Car(const Gridpoint& gp, int road, int lane) : Vehicle(gp, road, lane, Vehicle::VCONST[CAR]) {};

//Creates a van
Van::Van(const Gridpoint& gp, int road, int lane) : Vehicle(gp, road, lane, Vehicle::VCONST[VAN]) {};

//Creates a motercycle
Motercycle::Motercycle(const Gridpoint& gp, int road, int lane) : Vehicle(gp, road, lane, Vehicle::VCONST[MOTERCYCLE]) {};

//Creates a 12-wheeler truck
Truck::Truck(const Gridpoint& gp, int road, int lane) : Vehicle(gp, road, lane, Vehicle::VCONST[TRUCK]) {};

//Simulates one unit of time for the vehicle
void Vehicle::tick() {
    //continue whatever this car was doing
}

//sets the environment in which this vehicle is in
void Vehicle::setEnvironment(const Intersection* intersection) {
    localEnvironment = intersection;
}

//checks the traffic signal in front of the car
bool Vehicle::checkSignal() const {
    //this should check if the signal in front of them allows them to go through the intersection
    //green = go, red = stop, yellow = calculate it!
    //true means go, false means stop
    return true;
}

//checks if there are pedstrians walking in the way of a right turn
bool Vehicle::checkRightPedestrians() const {
    //check if there are pedestrains walking where this car would turn right
    return true;
}

//accelerates or decelerates
void Vehicle::accelerate(bool faster) {
    //accelerate if true, decelerate if false
}

//checks if the vehicle is on track to run a red light
bool Vehicle::goingToRunRedLight() const {
    //check if this guys gonna run a red light. take into account yellows!
    return false;
}

//checks if the vehicle is on track to read-end someone
bool Vehicle::goingToRearEnd() const {
    //check if this guys gonna read end the guy in front of him. Take into account their speed
    return false;
}

//Returns true if this vehicle is safe to continue at its speed
bool Vehicle::tooFast() const {
    return !this->goingToRunRedLight() && !this->goingToRearEnd();
}