#include "Throttle.h"
#include "variables.h"
#include "my_math.h"
#include "utils.h"

static float throttleRamped = 0.0;
static float SpeedFiltered = 0.0; //Limitation de la vitesse max

static float regenlim =0;

#define PedalPosArrLen 50
static float PedalPos;
static float LastPedalPos;
static float PedalChange =0;
static float PedalPosTot =0;
static float PedalPosArr[PedalPosArrLen];
static uint8_t PedalPosIdx = 0;
static int8_t PedalReq = 0; //positive is accel negative is decell

Throttle::Throttle(Variables * variables){

    this -> variables = variables;
}

bool Throttle::CheckAndLimitRange(int* potval, int potIdx){
    // The range check accounts for inverted throttle pedals, where the minimum
    // value is higher than the maximum. To accomodate for that, the potMin and potMax
    // variables are set for internal use.
    int potMin = potmax[potIdx] > potmin[potIdx] ? potmin[potIdx] : potmax[potIdx];
    int potMax = potmax[potIdx] > potmin[potIdx] ? potmax[potIdx] : potmin[potIdx]; ///replace this for our usecase

    if (((*potval + POT_SLACK) < potMin) || (*potval > (potMax + POT_SLACK)))
    {
        *potval = potMin;
        return false;
    }
    else if (*potval < potMin)
    {
        *potval = potMin;
    }
    else if (*potval > potMax)
    {
        *potval = potMax;
    }

    return true;


}

float Throttle::NormalizeThrottle(int potval, int potIdx)
{
    if(potIdx < 0 || potIdx > 1)
        return 0.0f;

    if(potmin[potIdx] == potmax[potIdx])
        return 0.0f;


    return 100.0f * ((float)(potval - potmin[potIdx]) / (float)(potmax[potIdx] - potmin[potIdx]));
}



/**
 * @brief Calculate a throttle percentage from the potval input.
 *
 * After the previous range checks, the throttle input potval lies within the
 * range of [potmin[0], potmax[0]]. From this range, the input is converted to
 * a percent range of [-100.0, 100.0].

 *
 * TODO: No regen implemented. Commanding 0 throttle while braking, otherwise direct output.
 *
 * @param potval
 * @param idx Index of the throttle input that should be used for calculation.
 * @param brkpedal Brake pedal input (true for brake pedal pressed, false otherwise).
 * @return float
 */
float Throttle::CalcThrottle(int potval, int potIdx, bool brkpedal)
{
    int speed = variables->getInt(SPEED);
    int dir = variables->getInt(DIRECTION);
    float potnom = 0.0f;  // normalize potval against the potmin and potmax values

    if(speed < 0)//make sure speed is not negative
    {
        speed *= -1;
    }

    //limiting speed change rate(pas d'accouts de vitesse)
    if(ABS(speed-SpeedFiltered)>ThrotRpmFilt)   ///max change is ThrotRpmFilt value
    {
        if(speed > SpeedFiltered)
        {
            SpeedFiltered +=  ThrotRpmFilt;
        }
        else
        {
            SpeedFiltered -=  ThrotRpmFilt;
        }
    }
    else
    {
        SpeedFiltered = speed;
    }

    speed = SpeedFiltered;

    ///////////////////////

    if(dir == 0)//neutral no torque command
    {
        return 0;
    }

    if (brkpedal)  //if break is pressed, we must finish here and return a negative value.
    {
        if(speed < 100 || speed < regenendRpm)
        {
            return 0; //noregen, too slow
        }
        else if (speed < regenRpm)
        {
            potnom = utils::change(speed, regenendRpm, regenRpm, 0, regenBrake);//taper regen according to speed
            return potnom;
        }
        else
        {   //very fast, max regen
            potnom =  regenBrake;
            return potnom;
        }
    }

    // substract offset, bring potval to the potmin-potmax scale and make a percentage
    potnom = NormalizeThrottle(potval, potIdx); //to bring it between 0 and 100

    // Apply the deadzone parameter. To avoid that we lose the range between
    // 0 and throtdead, the scale of potnom is mapped from the [0.0, 100.0] scale
    // to the [throtdead, 100.0] scale.
    if(potnom < throtdead)
    {
        potnom = 0.0f;          ///throtdead is the min accepted by the Inverter
    }
    else
    {
        potnom = (potnom - throtdead) * (100.0f / (100.0f - throtdead));
    }

//!! pedal command intent coding

    PedalPos = potnom; //save comparison next time to check if pedal had moved

    float TempAvgPos = AveragePos(PedalPos); //get an rolling average pedal position over the last 50 measurements for smoothing

    PedalChange = PedalPos - TempAvgPos; //current pedal position compared to average


    if(PedalChange < -1.0 )//Check pedal is release compared to last time
    {
        PedalReq = -1; //pedal is released enough - Commanding regen or slowing
    }
    else if(PedalChange > 1.0 )//Check pedal is increased compared to last time
    {
        PedalReq = 1; //pedal pressed - Commanding accelerating - thus always more power
    }
    else//pedal not changed
    {
        potnom = TempAvgPos; //use the averaged pedal
    }


    //Do clever bits for regen and such.


    if(speed < 100 || speed <regenendRpm)//No regen under 100 rpm or speed under regenendRpm
    {
        regenlim = 0;
    }
    else if(speed < regenRpm)
    {
        regenlim = utils::change(speed, regenendRpm, regenRpm, 0, regenmax);//taper regen according to speed
    }
    else
    {
        regenlim = regenmax;
    }


    //!!!potnom is throttle position up to this point//

    if(dir == 1)//Forward
    {
        //change limits to uint32, multiply by 10 then 0.1 to add a decimal to remove the hard edges
        potnom = utils::changeFloat(potnom,0,100,regenlim*10,throtmax*10);
        potnom *= 0.1;
    }
    /*else //Reverse, as neutral already exited function
    {
        if(Param::GetInt(Param::revRegen) == 0)//If regen in reverse is to be off
        {
            regenlim = 0;
        }
        potnom = utils::changeFloat(potnom,0,100,regenlim*10,throtmaxRev*10);
        potnom *= 0.1;
    }*/

    LastPedalPos = PedalPos; //Save current pedal position for next loop. //inutilisé
    return potnom;
}