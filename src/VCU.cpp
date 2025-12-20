#include "VCU.h"
#include "variables.h"
#include "Throttle.h"
#include <iostream>
#include <cassert>
#include "utils.h"
#include "Inverter.h"

#include "DigSigMonitor.h"
#include "DigSignal.h"

#include "PotFileParser.h"
#include "DigFileParser.h"
#include <cstring>

using namespace std;

static Inverter inverter;
static Inverter *selectedInverter = &inverter;

#ifdef TEST

DigFileParser IMDParser("/home/ehasbani/IRT/VCU-IRT/data/DataIMD.csv");
DigFileParser R2DParser("/home/ehasbani/IRT/VCU-IRT/data/DataR2D.csv");
DigFileParser BSPDParser("/home/ehasbani/IRT/VCU-IRT/data/DataBSPD.csv");
DigFileParser TSMSParser("/home/ehasbani/IRT/VCU-IRT/data/DataTSMS.csv");

static DigSignal TSMS(&TSMSParser);
static DigSignal IMD(&IMDParser);
static DigSignal BSPD(&BSPDParser);
static DigSignal R2D(&R2DParser);

static DigSignal *selectedTSMS = &TSMS;
static DigSignal *selectedIMD = &IMD;
static DigSignal *selectedBSPD = &BSPD;
static DigSignal *selectedR2D = &R2D;

#else

static DigSignal TSMS;
static DigSignal IMD;
static DigSignal BSPD;
static DigSignal R2D;

static DigSignal *selectedTSMS = &TSMS;
static DigSignal *selectedIMD = &IMD;
static DigSignal *selectedBSPD = &BSPD;
static DigSignal *selectedR2D = &R2D;

#endif

Variables *variables;
FileParser parser("/home/ehasbani/IRT/VCU-IRT/data/dataPot.csv");
void VCU::init()
{
    createAndAddSignals();

    variables = new Variables;
    init_throttle_test_set_1();
    init_utils();
}

void VCU::Task10ms()
{
    cout << "started Task10ms\n";
    variables->setInt(SPEED, 5);
    variables->setInt(DIRECTION, 1);

    cout << (variables->getInt(SPEED)) << "\n";

    cout << "-----------Throttle test set 1-------------\n";
    int potval = 1900;
    float expected_value = 46.2943;
    cout << "expected value with potval=" << potval << " is " << expected_value << "\n";
    float actual_value = Throttle::CalcThrottle(potval, 0, 0);
    cout << "actual computed value is " << actual_value << "\n\n";
    // assert(expected_value == actual_value);
    // printf("test passed");

    cout << "-----------Throttle test set 1-------------\n";

    cout << utils::GetUserThrottleCommand() << "\n";
    cout << utils::GetUserThrottleCommand() << "\n";

    // TODOS
    // update speed for next iteration(get speed form inverter class)
    // update temprature for next iteration(get itfrom motor and inverter)

    DigSigMonitor::checkDigSignals1ms();
    DigSigMonitor::checkDigSignals10ms();
    printf("checked dig signals\n");

    printf(selectedTSMS->getState() ? "TSMS HIGH\n" : "TSMS LOW\n");
    printf(selectedR2D->getState() ? "R2D HIGH\n" : "R2D LOW\n");
}

void VCU::receiveCanCallback(uint32_t id, uint32_t data[2], uint8_t length)
{
    selectedInverter->DecodeCanMessage(id, data);
}

void VCU::createAndAddSignals()
{
    DigSignal *signalmsToAdd[] = {selectedTSMS, selectedIMD, selectedBSPD};
    addSignalsToMonitor("1ms", signalmsToAdd, 3);

    DigSignal *signal10msToAdd[] = {selectedR2D};
    addSignalsToMonitor("10ms", signal10msToAdd, 1);
}

void VCU::addSignalsToMonitor(char *queue, DigSignal **signals, int count)
{
    // add signals to the appropriate monitor queue based on the string
    void (*addFunc)(DigSignal *);

    if (!strcmp(queue, "1ms"))
    {
        printf("adding 1ms signals count:%d\n", count);
        addFunc = DigSigMonitor::add1msSignal;
    }
    else if (!strcmp(queue, "10ms"))
    {
        printf("adding 10ms signals count:%d\n", count);
        addFunc = DigSigMonitor::add10msSignal;
    }

    for (int i = 0; i < count; ++i)
    {
        addFunc(signals[i]);
    }
}

void VCU::init_utils()
{
    utils::variables = variables;
    utils::parser = &parser;
}

void VCU::init_throttle_test_set_1()
{
    Throttle::variables = variables;

    Throttle::potmin[0] = 100;  // ADC raw min for pedal sensors
    Throttle::potmin[1] = 100;  // ADC raw min for pedal sensors
    Throttle::potmax[0] = 3900; // ADC raw max for pedal se
    Throttle::potmax[1] = 3900; // ADC raw max for pedal se

    Throttle::regenRpm = 1000.0f;   // Start applying regen above this RPM
    Throttle::regenendRpm = 200.0f; // Regen fades out below this RPM
    Throttle::brknompedal = 0.0f;   // Neutral brake pedal level
    Throttle::regenmax = 30.0f;     // Max regen % (negative torque)
    Throttle::regenBrake = 50.0f;   // Regen when brake pedal pressed (%)
    Throttle::brkcruise = 0.0f;     // Brake threshold to cancel c

    Throttle::idleSpeed = 800; // Target idle speed in rpm
    Throttle::cruiseSpeed = 0; // Default cruise setpoint (none)
    Throttle::speedkp = 0.2f;  // PID proportional gain for cruise
    Throttle::speedflt = 50;   // Filter time constant for speed

    Throttle::idleThrotLim = 5.0f; // Throttle limit during idle hold (%)
    Throttle::throtmax = 100.0f;   // Max throttle %
    Throttle::throtmaxRev = 40.0f; // Max throttle in reverse %
    Throttle::throtmin = 0.0f;     // Min throttle limit (%)
    Throttle::throtdead = 2.0f;    // Deadband around 0% pedal inpu

    Throttle::regenRamp = 2.0f;    // %/10ms rate of change for regen
    Throttle::throttleRamp = 3.0f; // %/10ms rate of change for thr

    Throttle::udcmin = 250.0f;  // Min DC bus voltage (V)
    Throttle::udcmax = 410.0f;  // Max DC bus voltage (V)
    Throttle::idcmin = -400.0f; // Min DC current (regen limit, A)
    Throttle::idcmax = 400.0f;  // Max DC current (drive limi

    Throttle::speedLimit = 160;     // km/h speed limiter
    Throttle::ThrotRpmFilt = 50.0f; // Filtered RPM used for throttle logic
}