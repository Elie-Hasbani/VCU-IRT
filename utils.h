#include <cstdint>

#ifndef UTILS_H
#define UTILS_H
class utils
{
public:
    //VCU();
    uint16_t ProcessThrottle(uint16_t prevSpeed);
    float GetUserThrottleCommand();

    inline static int32_t change(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    inline static float changeFloat(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    
    





};

#endif