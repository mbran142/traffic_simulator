#include "vehicle.h"

//declaring table of vehicle accelerations, max speeds, and sizes
const int Vehicle::VCONST[SIZE_VEHICLES][SIZE_CONSTS] = { { C_ACC, C_SPD, C_SIZE },
                                                          { V_ACC, V_SPD, V_SIZE },
                                                          { M_ACC, M_SPD, M_SIZE },
                                                          { T_ACC, T_SPD, T_SIZE } };

//COMMENT THIS
Vehicle::Vehicle(int road, int lane, const int consts[SIZE_CONSTS]) : ACCELERATION_MAX(consts[ACCELERATION]), SPEED_MAX(consts[SPEED]), VEHICLE_SIZE(consts[SIZE]) {

    this->road = road;
    this->lane = lane;

    //get initial speed / position
    //TODO: position = {struct position};

    speed = SPEED_MAX;
    while (this->tooFast())
        speed--;

    //TODO: decide whether to tick() or not
};

Car::Car(int road, int lane) : Vehicle(road, lane, Vehicle::VCONST[CAR]) {};
Van::Van(int road, int lane) : Vehicle(road, lane, Vehicle::VCONST[VAN]) {};
Motercycle::Motercycle(int road, int lane) : Vehicle(road, lane, Vehicle::VCONST[MOTERCYCLE]) {};
Truck::Truck(int road, int lane) : Vehicle(road, lane, Vehicle::VCONST[TRUCK]) {};

void Vehicle::tick() {
    //continue whatever this car was doing
}

bool Vehicle::checkSignal() const {
    //this should check if the signal in front of them allows them to go through the intersection
    //green = go, red = stop, yellow = calculate it!
    //true means go, false means stop
    return true;
}

bool Vehicle::checkRightPedestrians() const {
    //check if there are pedestrains walking where this car would turn right
    return true;
}

void Vehicle::accelerate(bool faster) {
    //accelerate if true, decelerate if false
}

bool Vehicle::goingToRunRedLight() const {
    //check if this guys gonna run a red light. take into account yellows!
    return false;
}

bool Vehicle::goingToRearEnd() const {
    //check if this guys gonna read end the guy in front of him. Take into account their speed
    return false;
}

//Returns true if this vehicle is safe to continue at its speed
bool Vehicle::tooFast() const {
    return !this->goingToRunRedLight() && !this->goingToRearEnd();
}