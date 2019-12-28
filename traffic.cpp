#include <iostream>
#include "intersection.h"

int totalTicks;

void testLaneData();

int main() {

    srand((unsigned)time(NULL));
    totalTicks = 0;

    Intersection intersection;

    std::cout << intersection << std::endl;

    return 0;
}

void testLaneData() {

    char grid[GRID_SIZE][GRID_SIZE], curChar = 'z';
    std::vector<std::string> errors;

    int TO_UPPER = 32;

    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
        grid[i / GRID_SIZE][i % GRID_SIZE] = '.';

    //in and out roads
    for (int i = 0; i < IN_OUT_COUNT; i++) {
        for (int j = 0; j < NUM_ROADS; j++) {
            for (int k = 0; k < NUM_LANES; k++) {
                grid[Road::LANE_LOC[i][j][k][0]][Road::LANE_LOC[i][j][k][1]] = curChar;
                grid[Road::LANE_LOC[i][j][k][2]][Road::LANE_LOC[i][j][k][3]] = curChar - TO_UPPER;
            }
        }
    }

    curChar = 'a';

    //intersection roads
    for (int i = 0; i < INTERSECTION_LANE_SIZE; i++) {
        for (int j = 0; j < NUM_ROADS; j++, curChar++) {
            grid[Road::INTERSECTION_LANE_LOC[i][j][0]][Road::INTERSECTION_LANE_LOC[i][j][1]] = curChar;
            grid[Road::INTERSECTION_LANE_LOC[i][j][2]][Road::INTERSECTION_LANE_LOC[i][j][3]] = curChar - TO_UPPER;
        }
    }

    for (int i = 0; i < GRID_SIZE; i++) {
        
        for (int j = 0; j < GRID_SIZE; j++)
            std::cout << grid[j][i] << ' ';
        
        std::cout << std::endl;
    }

    std::cout << std::endl;
    for (typename std::vector<std::string>::iterator itr = errors.begin(); itr != errors.end(); ++itr)
        std::cout << *itr << std::endl;
}