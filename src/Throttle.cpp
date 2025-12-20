#include "Throttle.h"
#include "variables.h"
#include "my_math.h"
#include "utils.h"

int Throttle::potmin[2];
int Throttle::potmax[2];
float Throttle::regenRpm;
float Throttle::regenendRpm;
float Throttle::brknompedal;
float Throttle::regenmax;
float Throttle::regenBrake;
float Throttle::brkcruise;
int Throttle::idleSpeed;
int Throttle::cruiseSpeed;
float Throttle::speedkp;
int Throttle::speedflt;
// int Throttle::speedFiltered;   //Lissage de la vitesse pour regen/throttle
float Throttle::idleThrotLim;
float Throttle::potnomFiltered;
float Throttle::throtmax;
float Throttle::throtmaxRev;
float Throttle::throtmin;
float Throttle::throtdead;
float Throttle::regenRamp;
float Throttle::throttleRamp;
float Throttle::udcmin;
float Throttle::udcmax;
float Throttle::idcmin;
float Throttle::idcmax;
int Throttle::speedLimit;
float Throttle::ThrotRpmFilt;
float UDCres;
float IDCres;
float UDCprevspnt = 0;
float IDCprevspnt = 0;

static float throttleRamped = 0.0;
static float SpeedFiltered = 0.0; // limitation des saut de vitesse

static float regenlim = 0;

#define PedalPosArrLen 50
static float PedalPos;
static float LastPedalPos;
static float PedalChange = 0;
static float PedalPosTot = 0;
static float PedalPosArr[PedalPosArrLen];
static uint8_t PedalPosIdx = 0;
static int8_t PedalReq = 0; // positive is accel negative is decell

bool Throttle::CheckAndLimitRange(int *potval, int potIdx)
{
    // The range check accounts for inverted throttle pedals, where the minimum
    // value is higher than the maximum. To accomodate for that, the potMin and potMax
    // variables are set for internal use.
    int potMin = potmin[potIdx];
    int potMax = potmax[potIdx];

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
    if (potIdx < 0 || potIdx > 1)
        return 0.0f;

    if (potmin[potIdx] == potmax[potIdx])
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
    int speed = Variables::getInt(SPEED);
    int dir = Variables::getInt(DIRECTION);
    float potnom = 0.0f; // normalize potval against the potmin and potmax values

    if (speed < 0) // make sure speed is not negative
    {
        speed *= -1;
    }

    // limiting speed change rate(pas d'accouts de vitesse)
    //  Si on recois des trames CAN éronnées, ou si il y as du bruit,les vcaleurs de speed recus seront fausses,donc les valeur de throttle calculés seront trop grands,
    // par rapport au précedante si speed ondule beaucou trop. c'est pour ca qu'on ramp le speed, meme si le couple calculé n'auras pas d'effet
    // ca evite de faire partie en vrille notre inverter a cause de bruit/glich CAN.
    if (ABS(speed - SpeedFiltered) > ThrotRpmFilt) /// max change is ThrotRpmFilt value
    {
        if (speed > SpeedFiltered)
        {
            SpeedFiltered += ThrotRpmFilt;
        }
        else
        {
            SpeedFiltered -= ThrotRpmFilt;
        }
    }
    else
    {
        SpeedFiltered = speed;
    }

    speed = SpeedFiltered;

    ///////////////////////

    if (dir == 0) // neutral no torque command
    {
        return 0;
    }

    if (brkpedal) // if break is pressed, we must finish here and return a negative value.
    {
        if (speed < 100 || speed < regenendRpm)
        {
            return 0; // noregen, too slow
        }
        else if (speed < regenRpm)
        {
            potnom = utils::change(speed, regenendRpm, regenRpm, 0, regenBrake); // taper regen according to speed
            return potnom;
        }
        else
        { // very fast, max regen
            potnom = regenBrake;
            return potnom;
        }
    }

    // substract offset, bring potval to the potmin-potmax scale and make a percentage
    potnom = NormalizeThrottle(potval, potIdx); // to bring it between 0 and 100

    // Apply the deadzone parameter. To avoid that we lose the range between
    // 0 and throtdead, the scale of potnom is mapped from the [0.0, 100.0] scale
    // to the [throtdead, 100.0] scale.
    if (potnom < throtdead)
    {
        potnom = 0.0f; /// throtdead is the min accepted by the Inverter
    }
    else
    {
        potnom = (potnom - throtdead) * (100.0f / (100.0f - throtdead));
    }

    //!! pedal command intent coding

    /*PedalPos = potnom; //save comparison next time to check if pedal had moved

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
    }*/

    // Do clever bits for regen and such.

    if (speed < 100 || speed < regenendRpm) // No regen under 100 rpm or speed under regenendRpm
    {
        regenlim = 0;
    }
    else if (speed < regenRpm)
    {
        regenlim = utils::change(speed, regenendRpm, regenRpm, 0, regenmax); // taper regen according to speed
    }
    else
    {
        regenlim = regenmax;
    }

    //!!!potnom is throttle position up to this point//

    if (dir == 1) // Forward
    {
        // change limits to uint32, multiply by 10 then 0.1 to add a decimal to remove the hard edges
        potnom = utils::changeFloat(potnom, 0, 100, regenlim * 10, throtmax * 10); ////most important line for regen calculation, will have a schematics to explain how it works.
        potnom *= 0.1;
    }

    LastPedalPos = PedalPos; // Save current pedal position for next loop. //inutilisé
    return potnom;
}

/*void Throttle::UdcLimitCommand(float& finalSpnt, float udc)
{
    udcmin = Param::GetFloat(Param::udcmin);//Made dynamic
    udcmax = Param::GetFloat(Param::udclim);

    uint16_t DerateReason = Param::GetInt(Param::TorqDerate);

    if(udcmin>0)    //ignore if set to zero. useful for bench testing without isa shunt
    {
        if (finalSpnt >= 0) //if we are requesting torque
        {
            float udcErr = udc - udcmin;
            UDCres = udcErr * 3.5; // error multiplied by
            UDCres = MAX(0, UDCres); // only allow positive UDCres limit
            if(finalSpnt > UDCres) //derate on udcmin
            {
                DerateReason |= 1;
                Param::SetInt(Param::TorqDerate,DerateReason);
            }
            finalSpnt = MIN(finalSpnt, UDCres);
        }
        else
        {
            float udcErr = udc - udcmax;
            UDCres = udcErr * 3.5;
            UDCres = MIN(0, UDCres);
            if(finalSpnt < UDCres)//derate on udcmax
            {
                DerateReason |= 2;
                Param::SetInt(Param::TorqDerate,DerateReason);
            }
            finalSpnt = MAX(finalSpnt, UDCres);

        }
    }
    else
    {
        finalSpnt = finalSpnt;
    }
}

void Throttle::IdcLimitCommand(float& finalSpnt, float idc)
{
    static float idcFiltered = 0;
    idcFiltered = IIRFILTERF(idcFiltered, idc, 4);

    idcmax = Param::GetFloat(Param::idcmax);//Made dynamic
    idcmin = Param::GetFloat(Param::idcmin);

    uint16_t DerateReason = Param::GetInt(Param::TorqDerate);

    if(idcmax>0)    //ignore if set to zero. useful for bench testing without isa shunt
    {
        if (finalSpnt >= 0)
        {
            float idcerr = idcmax - idcFiltered;
            IDCres = idcerr * 1;//gain needs tuning
            IDCres = MAX(0, IDCres);
            if(finalSpnt > IDCres)//derate on idcmax
            {
                DerateReason |= 8;
                Param::SetInt(Param::TorqDerate,DerateReason);
            }
            finalSpnt = MIN(finalSpnt, IDCres);
        }
        else
        {

            float idcerr = idcmin + idcFiltered;
            IDCres = idcerr * 1;//gain needs tuning
            IDCres = MIN(0, IDCres);
            if(finalSpnt < IDCres)//derate on idcmin
            {
                DerateReason |= 4;
                Param::SetInt(Param::TorqDerate,DerateReason);
            }
            finalSpnt = MAX(finalSpnt, IDCres);
        }
    }
    else
    {
        finalSpnt = finalSpnt;
    }
}*/

void Throttle::SpeedLimitCommand(float &finalSpnt, int speed)
{
    static int speedFiltered = 0;

    speedFiltered = IIRFILTER(speedFiltered, speed, 4);

    if (finalSpnt > 0) // Only limit if driver is asking for positive torque (acceleration)
    {
        int speederr = speedLimit - speedFiltered; // How far below the speed limit we are
        int res = speederr / 4;                    // Scale the error down

        res = MAX(0, res);               // Never allow negative (no throttle if over speed limit)
        finalSpnt = MIN(res, finalSpnt); // Clamp driver’s request down to res if necessary
    }
}

bool Throttle::TemperatureDerate(float temp, float tempMax, float &finalSpnt)
{
    uint16_t DerateReason = Variables::getInt(DERATE_REASON);
    float limit = 0;

    if (temp <= tempMax) // temperature low, allow full request
    {
        limit = 100.0f;
    }
    else if (temp < (tempMax + 2.0f)) // high, limit to 50% of max troque
    {
        limit = 50.0f;
        DerateReason |= 16;
        Variables::setInt(DERATE_REASON, DerateReason);
    } // else temp way too high and limit = 0

    if (finalSpnt >= 0)
        finalSpnt = MIN(finalSpnt, limit);
    else
        finalSpnt = MAX(finalSpnt, -limit);

    return limit < 100.0f;
}