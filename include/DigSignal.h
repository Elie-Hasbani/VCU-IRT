#include <cstdint>
#ifndef DIG_SIGNAL_H
#define DIG_SIGNAL_H

// typedef void (*CallBack)();
#include "DigFileParser.h"

class DigSignal
{
public:
    friend class DigSigMonitor;
    // digio object
    //
    // Signal(/*digio object,*/ CallBack cbHigh, void (*cbLow)()) : CallBackHigh(cbHigh), CallBackLow(cbLow) {};
    // void HandleHigh() { CallBackHigh(); }
    // void HandleLow() { CallBackLow(); };
    bool getState() { return state; }

#ifdef TEST
    void updateValue() { digParser->readNextEntry(this->state); }
    DigSignal(DigFileParser *path) : digParser(path) {}
    DigSignal() {}
#else
    void updateValue() { state = 0; }
#endif

private:
    // digio object
    //
    // CallBack CallBackHigh;
    // CallBack CallBackLow;
    bool state;
    uint32_t time_stamp;
    DigFileParser *digParser;
};

#endif // SIGNAL_H