#ifndef _SIGNAL_H
#define _SIGNAL_H

#include "traffic.h"

class TrafficSignal {
public:

private:
};

class SignalSystem {
public:

private:
    TrafficSignal leftSignal[NUM_SIGNAL_SETS],
                  middleSignal[NUM_SIGNAL_SETS],
                  rightSignal[NUM_SIGNAL_SETS];
};

#endif