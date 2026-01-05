
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
    // Add more as needed
};

// ints
// #define SPEED 0
// #define DIRECTION 1
/*#define BRAKE 2
#define OP_MODE 3
#define POT_MODE 4
#define DERATE_REASON 5
#define THROTRAMPRPM 6

// floats
#define POT1 0
#define POT2 1
#define INVERTER_TEMP 2
#define MOTOR_TEMP 3
#define INVERTER_TEMP_MAX 4
#define MOTOR_TEMP_MAX 5
#define POTNOM 6
#define THROTRAMP 7
#define THROTRAMPMAX 8*/

class Variables
{
public:
    static void init();
    static int getInt(int index);
    static float getFloat(int index);
    static void setInt(int index, int value);
    static void setFloat(int index, float value);

    static int GetInt(VarIds ParamNum);
    static float GetFloat(VarIds ParamNum);
    static void SetInt(VarIds ParamNum, int ParamVal);
    static void SetFloat(VarIds ParamNum, float ParamVal);

private:
    static int intValues[10];
    static float floatValues[10];
    static const int nbIntVar = 10;
    static const int nbFloatVar = 10;

    static s32fp values[100];
};

#endif
