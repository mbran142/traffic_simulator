#include "intersection.h"

//Default constructor for a 4-way traffic intersection
Intersection::Intersection() {
    grid = new Grid();
    crossroad = new Crossroad(this);
    //signalSystem = new SignalSystem();
}

//destructor for intersection
Intersection::~Intersection() {
    delete grid;
    delete crossroad;
    //delete signalSystem;
}

//Runs a traffic simulation
void Intersection::runSimulation() {
    //TODO: implement this
    std::cout << "Testing testing 123" << std::endl;
}

//Prints intersection
std::ostream& operator<<(std::ostream& os, const Intersection& it) {

    std::string top, bot;
    //0 = none | 1 = top / right single | 2 = bot / left single | 3 = double
    int quadrant[2][2];
    int status, h, v;

    for (int i = PRINT_BORDER; i < PRINT_SIZE; i++) {

        quadrant[0][0] = quadrant[0][1] = quadrant[1][0] = quadrant[1][1] = EMPTY;

        top.clear();
        bot.clear();

        for (int j = PRINT_BORDER; j < PRINT_SIZE; j++) {

            //check for signalSystem.lightStatus(i,j) (where i = N E S W, j = L M R)

            status = Grid::drawRoadLine(i, j);
            v = status / 10;
            h = status % 10;

            /*if (!it.getGrid().isOpen(i, j))
                quadrant[0][0] = 'C';*/

            switch (v) {
                case TOP_LINE:    quadrant[0][1] += VERTICAL_LINE; break;
                case BOTTOM_LINE: quadrant[1][1] += VERTICAL_LINE; break;
                case BOTH_LINES:  quadrant[0][1] += VERTICAL_LINE;
                                  quadrant[1][1] += VERTICAL_LINE; break;
            }

            switch (h) {
                case RIGHT_LINE:  quadrant[1][1] += HORIZONTAL_LINE; break;
                case LEFT_LINE:   quadrant[1][0] += HORIZONTAL_LINE; break;
                case BOTH_LINES:  quadrant[1][1] += HORIZONTAL_LINE;
                                  quadrant[1][0] += HORIZONTAL_LINE; break;
            }
                
            top.push_back(Intersection::codeChar(quadrant[0][0]));
            top.push_back(Intersection::codeChar(quadrant[0][1]));
            bot.push_back(Intersection::codeChar(quadrant[1][0]));
            bot.push_back(Intersection::codeChar(quadrant[1][1]));
        }

        std::cout << top << std::endl << bot;

        if (i != PRINT_SIZE - 1)
            std::cout << std::endl;
    }

    return os;
}

//
char Intersection::codeChar(int code) {

    switch (code) {
        case EMPTY           : return ' ';
        case HORIZONTAL_LINE : return '-';
        case VERTICAL_LINE   : return '|';
        case BOTH_LINES      : return '+';
        //MORE DEPENDING ON VEHICLES AND LIGHTS
    }

    return 'X';
}

//returns a reference to the grid
Grid& Intersection::getGrid() const {
    return *grid;
}

