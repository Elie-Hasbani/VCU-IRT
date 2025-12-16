#include "DigitalSignalProcessing.h"

bool DigitalSignalProcessing::checkDigSignals1ms()
{
    for (int i = 0; i < size1ms; ++i)
    {
        signals1ms[i]->getSignal();
    }
}