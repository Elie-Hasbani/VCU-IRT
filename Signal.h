#include <cstdint>
#ifndef SIGNAL_H
#define SIGNAL_H

// typedef void (*CallBack)();

class Signal
{
public:
    // digio object
    //
    // Signal(/*digio object,*/ CallBack cbHigh, void (*cbLow)()) : CallBackHigh(cbHigh), CallBackLow(cbLow) {};
    // void HandleHigh() { CallBackHigh(); }
    // void HandleLow() { CallBackLow(); };
    void getSignal();

private:
    // digio object
    //
    // CallBack CallBackHigh;
    // CallBack CallBackLow;
    bool state;
    uint32_t time_stamp;
};

#endif // SIGNAL_H