#include <cstdint>
#ifndef DIG_SIGNAL_H
#define DIG_SIGNAL_H

// typedef void (*CallBack)();

class DigSignal
{
public:
    friend class DigSigMonitor;
    // digio object
    //
    // Signal(/*digio object,*/ CallBack cbHigh, void (*cbLow)()) : CallBackHigh(cbHigh), CallBackLow(cbLow) {};
    // void HandleHigh() { CallBackHigh(); }
    // void HandleLow() { CallBackLow(); };
    bool getSignal() { return 1; };
    bool getState() { return state; };

private:
    // digio object
    //
    // CallBack CallBackHigh;
    // CallBack CallBackLow;
    bool state;
    void setState(bool state) { this->state = state; }
    uint32_t time_stamp;
};

#endif // SIGNAL_H