#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

#include "Signal.h"
#include "project_config.h"
class DigitalSignalProcessing
{
public:
    static bool checkDigSignals1ms(); // to be called periodically

    void add1msSignal(Signal *signal)
    {
        signals1ms[size1ms] = signal;
        ++size1ms;
    }

    static Signal *signals1ms[MAX_1MS_DIG_INPUTS];
    static Signal *signals10ms[MAX_10MS_DIG_INPUTS];
    static Signal *signals100ms[MAX_100MS_DIG_INPUTS];

    static int size1ms;
    static int size10ms;
    static int size100ms;
};

#endif // SIGNAL_PROCESSING_H