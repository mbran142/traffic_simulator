#ifndef _VEHICLE_H
#define _VEHICLE_H

#include "traffic.h"

class Intersection;
struct Gridpoint;
class Lane;

class Vehicle {
public:
    const static int VCONST[SIZE_VEHICLES][SIZE_CONSTS];
    Vehicle(const Gridpoint& gp, const Intersection*, const int[SIZE_CONSTS]);
    void tick();
    int getSize() const;
    static Vehicle* generateRandomVehicle(Gridpoint, const Intersection*);
protected:
    const int DECELERATION = ACCELERATION_MAX;
    const int ACCELERATION_MAX;
    const int SPEED_MAX;
    const int VEHICLE_SIZE;
    const int TURN_SPEED = SPEED_MAX / 2;
    const Intersection* itref;
    int acceleration, speed, destination;
    Lane* curLane;
    Gridpoint position;
    bool checkSignal() const;
    bool goingToRunRedLight() const;
    bool goingToRearEnd() const;
    bool tooFast() const;
};

class Car: public Vehicle {
public:
    Car(const Gridpoint&, const Intersection*);
};

class Van: public Vehicle {
public:
    Van(const Gridpoint&, const Intersection*);
};

class Motercycle: public Vehicle {
public:
    Motercycle(const Gridpoint&, const Intersection*);
};

class Truck: public Vehicle {
public:
    Truck(const Gridpoint&, const Intersection*);
};

#endif