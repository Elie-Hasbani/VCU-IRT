
#ifndef VCU_H
#define VCU_H

#include <stdint.h>
#include "DigSignal.h"
#include "Inverter.h"

class VCU
{
public:
    void init();
    void Task10ms();
    void receiveCanCallback(uint32_t id, uint32_t data[32], uint8_t length);
    void createAndAddSignals();

private:
    void init_throttle_test_set_1();
    void addSignalsToMonitor(char *queue, DigSignal **, int count);
    void init_utils();
};

#endif // VCU_H