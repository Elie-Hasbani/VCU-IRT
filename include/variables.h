
#ifndef VARIABLES_H
#define VARIABLES_H

#include "my_fp.h"
enum Op_Mode
{
    MODE_RUN,
    MODE_CHARGE
};
enum Pot_Mode
{
    MODE_ONE_POT,
    MODE_TWO_POT
};

enum class VarIds
{
    // Common
    SPEED,
    DIRECTION,
    BRAKE,
    OP_MODE,
    POT_MODE,
    DERATE_REASON,
    THROTRAMPRPM,
    POT1,
    POT2,
    INVERTER_TEMP,
    MOTOR_TEMP,
    INVERTER_TEMP_MAX,
    MOTOR_TEMP_MAX,
    POTNOM,
    THROTRAMP,
    THROTRAMPMAX,
    VOLTAGE,
    // Add more as needed
};

class Variables
{
public:
    static int GetInt(VarIds ParamNum);
    static float GetFloat(VarIds ParamNum);
    static void SetInt(VarIds ParamNum, int ParamVal);
    static void SetFloat(VarIds ParamNum, float ParamVal);

private:
    static s32fp values[100];
};

#endif
