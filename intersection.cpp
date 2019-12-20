#include "intersection.h"

//
void Intersection::runSimulation() {
    //TODO: implement this
    std::cout << "Testing testing 123" << std::endl;
}


//
std::ostream& operator<<(std::ostream& os, const Intersection& intersection) {
    //print intersection to console
    //probably just change the line to 'os << grid' and implement operator<<() for Grid
    os << "Test";
    return os;
}