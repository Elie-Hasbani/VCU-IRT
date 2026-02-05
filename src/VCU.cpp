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

void VCU::init()
{
    AddSignalsToQueue();
    initThrottleValues();
}

void VCU::Task10ms()
{
    cout << "-------------Task10ms-----------------\n";

    Variables::SetInt(VarIds::SPEED, selectedInverter->getMotorSpeed());
    Variables::SetFloat(VarIds::MOTOR_TEMP, selectedInverter->getMotorTemp());
    Variables::SetFloat(VarIds::INVERTER_TEMP, selectedInverter->getInvTemp());
    Variables::SetFloat(VarIds::VOLTAGE, selectedInverter->getVoltatge());

    cout << "test variables class " << Variables::GetInt(VarIds::SPEED) << "\n";

    cout << utils::ProcessThrottle(Variables::GetInt(VarIds::SPEED));

    // TODOS
    // update speed for next iteration(get speed form inverter class)
    // update temprature for next iteration(get itfrom motor and inverter)

    DigSigMonitor::checkDigSignals1ms();
    DigSigMonitor::checkDigSignals10ms();
    printf("checked dig signals\n");

    printf(selectedTSMS->getState() ? "TSMS HIGH\n" : "TSMS LOW\n");
    printf(selectedR2D->getState() ? "R2D HIGH\n" : "R2D LOW\n");

    cout << "-------------Task10ms-----------------\n\n";
}

void VCU::Task1ms()
{
    DigSigMonitor::checkDigSignals1ms();
}

void VCU::Task100ms()
{
    DigSigMonitor::checkDigSignals100ms();
}

void VCU::Task200ms()
{
    DigSigMonitor::checkDigSignals200ms();
}

void VCU::receiveCanCallback(uint32_t id, uint32_t data[2], uint8_t length)
{
    selectedInverter->DecodeCanMessage(id, data);
}

void VCU::AddSignalsToQueue()
{
    DigSignal *signalmsToAdd[] = {selectedTSMS, selectedIMD, selectedBSPD};
    char queue1ms[] = "1ms";
    addSignalsToMonitor(queue1ms, signalmsToAdd, 3);

    char queue10ms[] = "10ms";
    DigSignal *signal10msToAdd[] = {selectedR2D};
    addSignalsToMonitor(queue10ms, signal10msToAdd, 1);
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
    else
    {
        printf("Unknown queue: %s\n", queue);
        return;
    }

    for (int i = 0; i < count; ++i)
    {
        addFunc(signals[i]);
    }
}

void VCU::initParams()
{
    Variables::SetFloat(VarIds::THROTRAMP, 3.0f);          // throttle ramp rate(max value increase of thruttle) (%/10ms)
    Variables::SetFloat(VarIds::THROTRAMPMAX, 5.0f);       // throttle ramp rate at high RPM(max value increase of thruttle at high speed) (%/10ms)
    Variables::SetInt(VarIds::THROTRAMPRPM, 1000);         // rpm above which to use THROTRAMPMAX
    Variables::SetFloat(VarIds::MOTOR_TEMP_MAX, 80.0f);    // max motor temperature before limiting throttle(deg C)
    Variables::SetFloat(VarIds::INVERTER_TEMP_MAX, 85.0f); // max inverter temperature before limiting throttle(deg C)
    Variables::SetInt(VarIds::POT_MODE, MODE_TWO_POT);     // pot mode: one pot or two pot
}

void VCU::initThrottleValues()
{
    Throttle::potmin[0] = 100;  // ADC raw min for pedal sensor 1
    Throttle::potmin[1] = 100;  // ADC raw min for pedal sensor 2
    Throttle::potmax[0] = 3900; // ADC raw max for pedal sensor 1
    Throttle::potmax[1] = 3900; // ADC raw max for pedal sensor 2

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