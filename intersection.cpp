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

    long wait;

    std::cout << *this << std::endl;

    while (true) {

        wait = time(NULL);

        this->tick();
        std::cout << *this << std::endl;

        //wait ~one second
        do { ; } while (wait == time(NULL));
    }
}

//Prints intersection
std::ostream& operator<<(std::ostream& os, const Intersection& it) {

    std::string top, bot;
    //0 = none | 1 = top / right single | 2 = bot / left single | 3 = double
    int quadrant[2][2];
    Gridpoint status;
    int h, v;

    //print top indeces
    if (DEBUG) {
        std::string botLine;
        for (int i = PRINT_BORDER; i < PRINT_SIZE; i++) {
            top.push_back(i % 10 ? ' ' : i / 10 + '0');
            top.push_back(' ');
            bot.push_back(i % 10 + '0');
            bot.push_back(' ');
            botLine.push_back(i == PRINT_BORDER ? '+' : '-');
            botLine.push_back('-');
        }
        std::cout << "  " << top << std::endl << "  "<< bot << std::endl << "  " << botLine << std::endl;;
    }

    //y axis
    for (int j = PRINT_BORDER; j < PRINT_SIZE; j++) {

        top.clear();
        bot.clear();

        //print left indeces
        if (DEBUG) {
            top.push_back(j / 10 + '0');
            top.push_back(j % 10 + '0');
            top.push_back('|');
            bot += "  |";
        }

        //x axis
        for (int i = PRINT_BORDER; i < PRINT_SIZE; i++) {

            quadrant[0][0] = quadrant[0][1] = quadrant[1][0] = quadrant[1][1] = EMPTY;

            //check for signalSystem.lightStatus(i,j) (where i = N E S W, j = L M R)

            status = Grid::drawRoadLine(i, j);
            v = status.x;
            h = status.y;

            /*if (!it.getGrid().isOpen(i, j))
                quadrant[0][0] = 'C';*/


            /*
            - lanes will have an array of spaces that keep track of vehicles
            - through the lanes, each vehicle will move
            */

            switch (v) {
                case TOP_LINE:    quadrant[0][1] = VERTICAL_LINE; break;
                case BOTTOM_LINE: quadrant[1][1] = VERTICAL_LINE; break;
                case BOTH_LINES:  quadrant[0][1] = VERTICAL_LINE;
                                  quadrant[1][1] = VERTICAL_LINE; break;
            }

            switch (h) {
                case RIGHT_LINE:  quadrant[1][1] = quadrant[1][1] == EMPTY ? HORIZONTAL_LINE : BOTH_LINES ; break;
                case LEFT_LINE:   quadrant[1][0] = HORIZONTAL_LINE; break;
                case BOTH_LINES:  quadrant[1][1] = quadrant[1][1] == EMPTY ? HORIZONTAL_LINE : BOTH_LINES ;
                                  quadrant[1][0] = HORIZONTAL_LINE; break;
            }
                
            top.push_back(Intersection::codeChar(quadrant[0][0]));
            top.push_back(Intersection::codeChar(quadrant[0][1]));
            bot.push_back(Intersection::codeChar(quadrant[1][0]));
            bot.push_back(Intersection::codeChar(quadrant[1][1]));
        }

        std::cout << top << std::endl << bot;

        if (j != PRINT_SIZE - 1)
            std::cout << std::endl;
    }

    return os;
}

//returns a reference to the grid
Grid& Intersection::getGrid() const {
    return *grid;
}

//Represents a movement one unit of time (~1 second)
void Intersection::tick() {
    crossroad->tick();
    //signalSystem->tick();
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