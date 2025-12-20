


#include "variables.h"


#ifndef THROTTLE_H
#define THROTTLE_H 


#define POT_SLACK 200
class Throttle
{
public:
    //VCU();
    static float NormalizeThrottle(int potval, int potIdx);
    static bool CheckAndLimitRange(int* potval, int potidx);
    static void setMinpot(int pot1min, int pot2min);
    static void setMaxpot(int pot1max, int pot2max);
    static float CalcThrottle(int potval, int potIdx, bool brkpedal);

    static void UdcLimitCommand(float& finalSpnt, float udc);
    static void IdcLimitCommand(float& finalSpnt, float idc);
    static void SpeedLimitCommand(float& finalSpnt, int speed);
    static bool TemperatureDerate(float tmp, float tmpMax, float& finalSpnt);

    Throttle(Variables * variables);


    static Variables * variables;


    static int potmin[2];
    static int potmax[2];
    static float regenRpm;
    static float brknompedal;
    static float regenmax;
    static float regenBrake;
    static float brkcruise;
    static float throtmax;
    static float throtmaxRev;
    static float throtmin;
    static float throtdead;
    static int idleSpeed;
    static int cruiseSpeed;
    static float speedkp;
    static int speedflt;
    static float idleThrotLim;
    static float regenRamp;
    static float throttleRamp;
    static int bmslimhigh;
    static int bmslimlow;
    static int accelmax;
    static int accelflt;
    static float udcmin;
    static float udcmax;
    static float idcmin;
    static float idcmax;
    static int speedLimit;
    static float regenendRpm;
    static float ThrotRpmFilt;

    private:
        //static int speedFiltered;
        static float potnomFiltered;
        static float brkRamped;
        static float AveragePos(float Pos);

};

#endif