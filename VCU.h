
#include <stdint.h>
#include "DigSignal.h"

static DigSignal TSMS;
static DigSignal IMD;
static DigSignal BSPD;

static DigSignal *selectedTSMS = &TSMS;
static DigSignal *selectedIMD = &IMD;
static DigSignal *selectedBSPD = &BSPD;

class VCU
{
public:
    void init();
    void Task10ms();
    void receiveCanCallback(uint32_t id, uint32_t data[32], uint8_t length);

private:
    void init_throttle_test_set_1();
    void init_utils();
};