#include "vehicle.h"

const int Vehicle::VCONST[SIZE_VEHICLES][SIZE_CONSTS] = { { 2, 5, 2 },
                                                          { 2, 4, 3 },
                                                          { 2, 5, 1 },
                                                          { 1, 3, 8 } };

Vehicle::Vehicle(const int consts[SIZE_CONSTS]) : ACCELERATION_MAX(consts[ACCELERATION]), SPEED_MAX(consts[SPEED]), VEHICLE_SIZE(consts[SIZE]) {};

Car::Car() : Vehicle(Vehicle::VCONST[CAR]) {};
