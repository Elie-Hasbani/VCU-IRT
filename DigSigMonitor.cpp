#include "DigSigMonitor.h"

bool DigSigMonitor::checkDigSignals1ms()
{
    for (int i = 0; i < size1ms; ++i)
    {
        bool state = signals1ms[i]->getSignal();
        signals1ms[i]->setState(state);
    }
}