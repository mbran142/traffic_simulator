#ifndef _VEHICLE_H
#define _VEHICLE_H

#include "traffic.h"

class Intersection;
struct Gridpoint;

class Vehicle {
public:
    const static int VCONST[SIZE_VEHICLES][SIZE_CONSTS];
    Vehicle(const Gridpoint& gp,int, int, const int[SIZE_CONSTS]);
    void tick();
    void setEnvironment(const Intersection*);
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
    Car(const Gridpoint&, int, int);
};

class Van: public Vehicle {
public:
    Van(const Gridpoint&, int, int);
};

class Motercycle: public Vehicle {
public:
    Motercycle(const Gridpoint&, int, int);
};

class Truck: public Vehicle {
public:
    Truck(const Gridpoint&, int, int);
};

#endif