
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

private:
    void AddSignalsToQueue();
    void initThrottleValues();
    void addSignalsToMonitor(char *queue, DigSignal **, int count);
    void init_utils();
    void initParams();
};

#endif // VCU_H