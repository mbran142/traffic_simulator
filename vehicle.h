#ifndef _VEHICLE_H
#define _VEHICLE_H

#include "traffic.h"

class Intersection;
struct Gridpoint;
class Lane;

class Vehicle {
public:
    const static int VCONST[SIZE_VEHICLES][SIZE_CONSTS];
    Vehicle(Lane*, const Intersection*, const int[SIZE_CONSTS]);
    void tick();
    int getSize() const;
    static Vehicle* generateRandomVehicle(Lane*, const Intersection*);
protected:
    const int DECELERATION = ACCELERATION_MAX;
    const int ACCELERATION_MAX;
    const int SPEED_MAX;
    const int VEHICLE_SIZE;
    const int TURN_SPEED = SPEED_MAX / 2;
    const Intersection* itref;
    int acceleration, speed, destination, distanceLeftInLane, buttInLane;
    Lane *curLane, *buttLane;
    Gridpoint position, buttPosition;
    bool checkSignal() const;
    bool goingToRunRedLight() const;
    bool goingToRearEnd() const;
    bool tooFast() const;
};

class Car: public Vehicle {
public:
    Car(Lane*, const Intersection*);
};

class Van: public Vehicle {
public:
    Van(Lane*, const Intersection*);
};

class Motercycle: public Vehicle {
public:
    Motercycle(Lane*, const Intersection*);
};

class Truck: public Vehicle {
public:
    Truck(Lane*, const Intersection*);
};

#endif