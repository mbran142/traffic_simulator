#ifndef _VEHICLE_H
#define _VEHICLE_H

#include "traffic.h"
#include "road.h"

class Vehicle {
public:
    const static int VCONST[SIZE_VEHICLES][SIZE_CONSTS];
    Vehicle(int, int, const int[SIZE_CONSTS]);
    void tick();
protected:
    const int DECELERATION = 1;
    const int ACCELERATION_MAX;
    const int SPEED_MAX;
    const int VEHICLE_SIZE;
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
    Car(int, int);
};

class Van: public Vehicle {
public:
    Van(int, int);
};

class Motercycle: public Vehicle {
public:
    Motercycle(int, int);
};

class Truck: public Vehicle {
public:
    Truck(int, int);
};

#endif