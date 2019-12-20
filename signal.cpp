#include "signal.h"

//Takes the max time the light can be green and yellow
TrafficSignal::TrafficSignal(int GREEN_TIME, int YELLOW_TIME) : GREEN_TIME(GREEN_TIME), YELLOW_TIME(YELLOW_TIME) {
    status = RED;
    timeLeft = TrafficSignal::UNDETERMINED;
}

//Returns true if this signal is green
bool TrafficSignal::isGreen() {
    return status == GREEN;
}

//Returns true if this signal is yellow
bool TrafficSignal::isYellow() {
    return status == YELLOW;
}

//Returns true if this signal is red
bool TrafficSignal::isRed() {
    return status == RED;
}

//Ticks this signal one unit further in time
void TrafficSignal::tick(bool switchStatus) {
    
    //change status if given a signal or the time of its state has expired
    if (switchStatus || timeLeft == 0)
        this->cycleToNextStatus();

    //otherwise, decrement the time
    else if (timeLeft != TrafficSignal::UNDETERMINED)
        timeLeft--;
}

//Shift to next light color
void TrafficSignal::cycleToNextStatus() {

    switch (status) {

        case GREEN:
            status = YELLOW;
            timeLeft = YELLOW_TIME;
            break;

        case YELLOW:
            status = RED;
            timeLeft = TrafficSignal::UNDETERMINED;
            break;

        case RED:
            status = GREEN;
            timeLeft = GREEN_TIME;
            break;
    }
}


//
SignalSystem::SignalSystem() {
    for (int i = 0; i < NUM_SIGNAL_SETS; i++)
        for (int j = 0; j < NUM_SIGNAL_SETS; j++)
            //FIGURE OUT CONSTANTS
            signals[i][j] = new TrafficSignal(0,0);
}

//
SignalSystem::~SignalSystem() {
    for (int i = 0; i < NUM_SIGNAL_SETS; i++)
        for (int j = 0; j < NUM_SIGNAL_SETS; j++)
            delete signals[i][j];
}