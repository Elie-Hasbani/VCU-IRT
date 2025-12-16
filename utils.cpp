#include "utils.h"
#include "variables.h"
#include "Throttle.h"
#include <iostream>
#include "my_math.h"

using namespace std;

Variables *utils::variables;
FileParser *utils::parser;

float utils::GetUserThrottleCommand()
{
    bool brake = variables->getInt(BRAKE);
    int potMode = variables->getInt(POT_MODE);
    int direction = variables->getInt(DIRECTION);

    int v1, v2;
    parser->readPotVal1(v1);
    parser->readPotVal2(v2);
    std::cout << "potval1 = " << v1 << ", potval2 = " << v2 << endl;

    ////potval are analog values read from the throttle potentiometers
    int pot1val = v1;
    int pot2val = v2;
    // Param::SetInt(Param::pot, pot1val);
    // Param::SetInt(Param::pot2, pot2val);

    bool inRange1 = Throttle::CheckAndLimitRange(&pot1val, 0);
    bool inRange2 = Throttle::CheckAndLimitRange(&pot2val, 1);
    int useChannel = 0; // default case: use Throttle 1

    // when there's something wrong with the dual throttle values,
    // we try to make the best of it and use the valid one
    if (inRange1 && inRange2)
    {
        cout << "both in range\n";
        // These are only temporary values, because they can change
        // if the "limp mode" is activated.
        float pot1nomTmp = Throttle::NormalizeThrottle(pot1val, 0);
        float pot2nomTmp = Throttle::NormalizeThrottle(pot2val, 1);

        // potential limp mode if the two throttle values differ too much
        if (ABS(pot2nomTmp - pot1nomTmp) > 10.0f)
        {
            printf("too much difference\n");

            // simple implementation of a limp mode: select the lower of
            // the two throttle inputs and limiting the throttle value
            // to 50%
            if (pot1nomTmp < pot2nomTmp)
            {
                if (pot1nomTmp > 50.0f)
                    pot1val = Throttle::potmax[0] / 2;

                useChannel = 0;
            }
            else
            {
                if (pot2nomTmp > 50.0f)
                    pot2val = Throttle::potmax[1] / 2;

                useChannel = 1;
            }
        }
    }
    else if (inRange1 && !inRange2)
    {
        // utils::PostErrorIfRunning(ERR_THROTTLE2);
        cout << "pot1 in range pot2 not in range\n";
        useChannel = 0; // use throttle channel 1
    }
    else if (!inRange1 && inRange2)
    {
        cout << "pot1 not in range pot2 in range\n";
        // utils::PostErrorIfRunning(ERR_THROTTLE1);

        useChannel = 1; // use throttle channel 2
    }
    else // !inRange1 && !inRange2
    {
        cout << "pot1 not in range pot2 not in range\n";
        /// utils::PostErrorIfRunning(ERR_THROTTLE12);

        return 0.0;
    }

    // don't return a throttle value if we are in neutral
    // TODO: the direction for FORWARD/NEUTRAL/REVERSE needs an enum in param_prj.h as well
    if (direction == 0)
        return 0.0;

    if (direction == 2) // No throttle val if in PARK also.
        return 0.0;

    // calculate the throttle depending on the channel we've decided to use
    if (useChannel == 0)
        return Throttle::CalcThrottle(pot1val, 0, brake);
    else if (useChannel == 1)
        return Throttle::CalcThrottle(pot2val, 1, brake);
    else
        return 0.0;
}

float utils::ProcessThrottle(uint16_t speed)
{
    float finalSpnt;

    /*if (speed < Param::GetInt(Param::throtramprpm))
    {
        Throttle::throttleRamp = Param::GetFloat(Param::throtramp);
    }

    else
    {
        Throttle::throttleRamp = Param::GetAttrib(Param::throtramp)->max;
    }*/

    finalSpnt = utils::GetUserThrottleCommand();

    // Throttle::UdcLimitCommand(finalSpnt,Param::GetFloat(Param::udc));
    // Throttle::IdcLimitCommand(finalSpnt, ABS(Param::GetFloat(Param::idc)));
    Throttle::SpeedLimitCommand(finalSpnt, ABS(speed));

    if (Throttle::TemperatureDerate(variables->getFloat(INVERTER_TEMP), variables->getFloat(INVERTER_TEMP_MAX), finalSpnt))
    {
        cout << "inverter temp high";
    }

    if (Throttle::TemperatureDerate(variables->getFloat(MOTOR_TEMP), variables->getFloat(MOTOR_TEMP_MAX), finalSpnt))
    {
        cout << "motor temp high";
    }

    // finalSpnt = Throttle::RampThrottle(finalSpnt); //Move ramping as last step -intro V2.30A

    // make sure the torque percentage is NEVER out of range
    if (finalSpnt < -100.0f)
        finalSpnt = -100.0f;
    else if (finalSpnt > 100.0f)
        finalSpnt = 100.0f;

    variables->setFloat(POTNOM, finalSpnt);

    return finalSpnt;
}
