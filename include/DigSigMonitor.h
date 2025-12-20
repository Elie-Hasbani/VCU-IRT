#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

#include "DigSignal.h"
#include "project_config.h"
class DigSigMonitor
{
public:
    static void checkDigSignals1ms(); // to be called periodically

    static void add1msSignal(DigSignal *signal)
    {
        signals1ms[size1ms] = signal;
        ++size1ms;
    }

    static DigSignal **get1msList() { return signals1ms; }

    static DigSignal *signals1ms[MAX_1MS_DIG_INPUTS];
    static DigSignal *signals10ms[MAX_10MS_DIG_INPUTS];
    static DigSignal *signals100ms[MAX_100MS_DIG_INPUTS];

    static int size1ms;
    static int size10ms;
    static int size100ms;
};

#endif // SIGNAL_PROCESSING_H