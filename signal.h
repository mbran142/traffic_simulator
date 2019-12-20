#ifndef _SIGNAL_H
#define _SIGNAL_H

#include "traffic.h"

class TrafficSignal {
public:
    bool isGreen();
    bool isYellow();
    bool isRed();
    void tick();
private:
    static const int UNDETERMINED = -1;
    enum color { GREEN, YELLOW, RED };
    color status;
    int timeLeft;
    void cycleToNextStatus();
};

class SignalSystem {
public:
    SignalSystem();
    ~SignalSystem();
private:
    TrafficSignal signals[NUM_SIGNALS_PER_SET][NUM_SIGNAL_SETS];
};

#endif