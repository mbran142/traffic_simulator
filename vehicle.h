#ifndef _VEHICLE_H
#define _VEHICLE_H

#define SIZE_VEHICLES 4
#define CAR 0
#define VAN 1
#define MOTERCYCLE 2
#define TRUCK 3

#define SIZE_CONSTS 3
#define ACCELERATION 0
#define SPEED 1
#define SIZE 2 

class Vehicle {
public:
    const static int VCONST[SIZE_VEHICLES][SIZE_CONSTS];
    Vehicle(const int[SIZE_CONSTS]);
protected:
    const int ACCELERATION_MAX;
    const int SPEED_MAX;
    const int VEHICLE_SIZE;
    int speed;
    int position;
};

class Car: public Vehicle {
private:
public:
    Car();
};

#endif