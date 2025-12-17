#include "DigSigMonitor.h"
#include "DigSignal.h"
#include "init.h"
#include "VCU.h"

void init::createAndAddSignals()
{
    DigSigMonitor::add1msSignal(selectedTSMS);
    DigSigMonitor::add1msSignal(selectedIMD);
    DigSigMonitor::add1msSignal(selectedBSPD);
}