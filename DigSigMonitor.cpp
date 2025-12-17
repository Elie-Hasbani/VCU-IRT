#include "DigSigMonitor.h"
#include "DigSignal.h"
#include <cstdio>

DigSignal *DigSigMonitor::signals1ms[MAX_1MS_DIG_INPUTS] = {nullptr};
int DigSigMonitor::size1ms = 0;

void DigSigMonitor::checkDigSignals1ms()
{
    for (int i = 0; i < size1ms; ++i)
    {
        bool state = signals1ms[i]->getSignal();
        signals1ms[i]->setState(state);
    }
}