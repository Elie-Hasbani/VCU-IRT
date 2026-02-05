#include "DigSigMonitor.h"
#include "DigSignal.h"
#include <cstdio>

DigSignal *DigSigMonitor::signals1ms[MAX_1MS_DIG_INPUTS] = {nullptr};
DigSignal *DigSigMonitor::signals10ms[MAX_10MS_DIG_INPUTS] = {nullptr};
DigSignal *DigSigMonitor::signals100ms[MAX_100MS_DIG_INPUTS] = {nullptr};
DigSignal *DigSigMonitor::signals200ms[MAX_200MS_DIG_INPUTS] = {nullptr};

int DigSigMonitor::size1ms = 0;
int DigSigMonitor::size10ms = 0;
int DigSigMonitor::size100ms = 0;
int DigSigMonitor::size200ms = 0;

void DigSigMonitor::checkDigSignals1ms()
{
    for (int i = 0; i < size1ms; ++i)
    {
        signals1ms[i]->updateValue();
    }
}

void DigSigMonitor::checkDigSignals10ms()
{
    for (int i = 0; i < size10ms; ++i)
    {
        signals10ms[i]->updateValue();
    }
}

void DigSigMonitor::checkDigSignals100ms()
{
    for (int i = 0; i < size100ms; ++i)
    {
        signals100ms[i]->updateValue();
    }
}

void DigSigMonitor::checkDigSignals200ms()
{
    for (int i = 0; i < size200ms; ++i)
    {
        signals200ms[i]->updateValue();
    }
}