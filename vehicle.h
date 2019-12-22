#ifndef _VEHICLE_H
#define _VEHICLE_H

#include "traffic.h"

class Intersection;
struct Gridpoint;

class Vehicle {
public:
    const static int VCONST[SIZE_VEHICLES][SIZE_CONSTS];
    Vehicle(const Gridpoint& gp, const Intersection*, const int[SIZE_CONSTS]);
    void tick();
    static Vehicle* generateRandomVehicle(Gridpoint, const Intersection*);
protected:
    const int DECELERATION = 1;
    const int ACCELERATION_MAX;
    const int SPEED_MAX;
    const int VEHICLE_SIZE;
    const Intersection* localEnvironment;
    int road, lane;
    int speed;
    Gridpoint position;
    bool checkSignal() const;
    bool checkRightPedestrians() const;
    void accelerate(bool);
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