#include "utils.h"
#include "variables.h"
#include "Throttle.h"
#include "iostream"
#include "my_math.h"




/*float utils::GetUserThrottleCommand(){
    int pot1val = 2;
    int pot2val = 3;
    pot = pot1val;
    pot2 = pot2val;

    bool inRange1 = Throttle::CheckAndLimitRange(&pot1val, 0);
    bool inRange2 = Throttle::CheckAndLimitRange(&pot2val, 1);
    int useChannel = 0; // default case: use Throttle 1

    if (potMode == MODE_ONE_POT)
    {
        if(!inRange1)
        {
            //DigIo::err_out.Set();
            std::cout<<"not in range";
            potnom = 0;
            return 0.0;
        }

        useChannel = 0;
    }
    else if(potMode == MODE_TWO_POT)
    {
        // when there's something wrong with the dual throttle values,
        // we try to make the best of it and use the valid one
        if(inRange1 && inRange2)
        {
            // These are only temporary values, because they can change
            // if the "limp mode" is activated.
            float pot1nomTmp = Throttle::NormalizeThrottle(pot1val, 0);
            float pot2nomTmp = Throttle::NormalizeThrottle(pot2val, 1);

            if(ABS(pot2nomTmp - pot1nomTmp) > 10.0f)
            {
                printf("too much difference");

                // simple implementation of a limp mode: select the lower of
                // the two throttle inputs and limiting the throttle value
                // to 50%
                if(pot1nomTmp < pot2nomTmp)
                {
                    if(pot1nomTmp > 50.0f)
                        pot1val = Throttle::potmax[0] / 2;

                    useChannel = 0;
                }
                else
                {
                    if(pot2nomTmp > 50.0f)
                        pot2val = Throttle::potmax[1] / 2;

                    useChannel = 1;
                }
            }
        }
        else if(inRange1 && !inRange2)
        {
            //utils::PostErrorIfRunning(ERR_THROTTLE2);

            useChannel = 0; // use throttle channel 1
        }
        else if(!inRange1 && inRange2)
        {
            //utils::PostErrorIfRunning(ERR_THROTTLE1);

            useChannel = 1; // use throttle channel 2
        }
        else // !inRange1 && !inRange2
        {
            ///utils::PostErrorIfRunning(ERR_THROTTLE12);

            return 0.0;
        }
    }
    else // (yet) unknown throttle mode
    {
        //utils::PostErrorIfRunning(ERR_THROTTLEMODE);

        return 0.0;
    }

    // don't return a throttle value if we are in neutral
    // TODO: the direction for FORWARD/NEUTRAL/REVERSE needs an enum in param_prj.h as well
    if (direction == 0)
        return 0.0;

    if (direction == 2)//No throttle val if in PARK also.
        return 0.0;

    // calculate the throttle depending on the channel we've decided to use
    if (useChannel == 0)
        return Throttle::CalcThrottle(pot1val, 0, brake);
    else if(useChannel == 1)
        return Throttle::CalcThrottle(pot2val, 1, brake);
    else
        return 0.0;
}

}*/

/*uint16_t utils::ProcessThrottle(uint16_t prevSpeed){

    float finalSpnt;
    /*if (speed < Param::GetInt(Param::throtramprpm))
    {
        Throttle::throttleRamp = Param::GetFloat(Param::throtramp);
    }

    else
    {
        Throttle::throttleRamp = Param::GetAttrib(Param::throtramp)->max;
    }
    

    finalSpnt = utils::GetUserThrottleCommand();

}*/