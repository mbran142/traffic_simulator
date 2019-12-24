#ifndef _TRAFFIC_H
#define _TRAFFIC_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

//RATES OF OCCURENCES

//rates of certain cars
const int car_rate = 91,
          van_rate = 5,
          motercycle_rate = 2,
          truck_rate = 2;

//rates of turning right in the rightmost lane
const int right_turn_rate = 40;

//road.h
enum color { GREEN, YELLOW, RED };

#define START_END_COUNT 4
#define START_X 0
#define START_Y 1
#define END_X 2
#define END_Y 3

#define IN_OUT_COUNT 2
#define IN_LANE 0
#define OUT_LANE 1

#define NUM_LANES 3
#define LEFT 0
#define MIDDLE 1
#define RIGHT 2

//lanes
#define LANE_SIZE 26
#define INTERSECTION_SIZE 10

//grid size
#define GRID_SIZE (LANE_SIZE * 2 + INTERSECTION_SIZE)
#define PRINT_SIZE (GRID_SIZE - T_SIZE)

//roads
#define NUM_ROADS 4
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define NUM_SIGNALS_PER_SET 3
#define NUM_SIGNAL_SETS 4
#define NUM_LANES_PER_ROAD 3

//vehicle.h

//array access / size constants
#define SIZE_VEHICLES 4
#define CAR 0
#define VAN 1
#define MOTERCYCLE 2
#define TRUCK 3

#define SIZE_CONSTS 3
#define ACCELERATION 0
#define SPEED 1
#define SIZE 2

//car
#define C_ACC 2
#define C_SPD 5
#define C_SIZE 2

//van
#define V_ACC 2
#define V_SPD 4
#define V_SIZE 3

//motercycle
#define M_ACC 2
#define M_SPD 5
#define M_SIZE 1

//truck
#define T_ACC 1
#define T_SPD 3
#define T_SIZE 6

//gridpoint struct
struct Gridpoint {
    int x;
    int y;
    Gridpoint() {
        this->x = 0;
        this->y = 0;
    }
    Gridpoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
    Gridpoint(const Gridpoint& in) {
        this->x = in.x;
        this->y = in.y;
    }
    Gridpoint& operator=(const Gridpoint& g) {
        this->x = g.x;
        this->y = g.y;
        return *this;
    }
};

#endif