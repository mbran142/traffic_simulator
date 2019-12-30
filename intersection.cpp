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

//Runs a traffic simulation for a certain amount of ticks
void Intersection::runSimulation(int ticks) {

    long wait;

    std::cout << *this << std::endl;

    for (int i = 0; i < ticks; i++) {

        wait = time(NULL);

        this->tick();
        std::cout << *this << std::endl;

        //wait ~one second
        do { ; } while (wait == time(NULL));
    }
}

//Creates the string to print for an intersection print
std::string Intersection::createIntersectionString() const {

    std::stringstream top, bot, out;
    //0 = none | 1 = top / right single | 2 = bot / left single | 3 = double
    int quadrant[2][2];
    Gridpoint status;
    int h, v;

    //print top indeces
    if (DEBUG) {
        top << "  ";
        bot << "  ";
        for (int i = PRINT_BORDER; i < PRINT_SIZE; i++) {
            top << ' ' << i << ' ';
            bot << (i == PRINT_BORDER ? '+' : '-') << "---";
        }
        std::cout << top.str() << std::endl << bot.str() << std::endl;
    }

    //y axis
    for (int j = PRINT_BORDER; j < PRINT_SIZE; j++) {

        //clear stringstreams
        top.str(std::string());
        bot.str(std::string());

        //print left indeces
        if (DEBUG) {
            top << j << '|';
            bot << "  |";
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
                
            top << Intersection::codeChar(quadrant[0]); 
            bot << Intersection::codeChar(quadrant[1]);
        }

        out << top.str() << '\n' << bot.str();

        if (j != PRINT_SIZE - 1)
            out << '\n';
    }

    return out.str();
}

//Prints intersection
std::ostream& operator<<(std::ostream& os, const Intersection& it) {
    os << it.createIntersectionString() << std::endl;
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

//Gets a lane
Lane* Intersection::getLane(int in_turn_out, int left_right_middle, int dir) const {
    return crossroad->getLane(in_turn_out, left_right_middle, dir);
}

//Converts the grid into printable characters
std::string Intersection::codeChar(int code[]) {

    std::stringstream out;

    for (int i = 0; i < 2; i++) {
        switch (code[i]) {
            case EMPTY           : out << "  "; break;
            case HORIZONTAL_LINE : out << "--"; break;
            case VERTICAL_LINE   : out << " |"; break;
            case BOTH_LINES      : out << " +"; break;
            //MORE DEPENDING ON VEHICLES AND LIGHTS
        }
    }

    return out.str();
}