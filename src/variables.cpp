#include "variables.h"
#include "my_fp.h"
#include <iostream>

using namespace std;

// Define static members
int Variables::intValues[10];
float Variables::floatValues[10];

s32fp Variables::values[100];

void Variables::init()
{
    for (int i = 0; i < nbIntVar; ++i)
    {
        intValues[i] = 0;
    }
    for (int i = 0; i < nbFloatVar; ++i)
    {
        floatValues[i] = 0;
    }
}

int Variables::getInt(int index)
{
    return intValues[index];
}

float Variables::getFloat(int index)
{
    return floatValues[index];
}

void Variables::setInt(int index, int value)
{
    intValues[index] = value;
}

void Variables::setFloat(int index, float value)
{
    floatValues[index] = value;
}

int Variables::GetInt(VarIds ParamNum)
{
    return FP_TOINT(values[static_cast<int>(ParamNum)]);
}

float Variables::GetFloat(VarIds ParamNum)
{
    return FP_TOFLOAT(values[static_cast<int>(ParamNum)]);
}

void Variables::SetInt(VarIds ParamNum, int ParamVal)
{
    values[static_cast<int>(ParamNum)] = FP_FROMINT(ParamVal);
}

void Variables::SetFloat(VarIds ParamNum, float ParamVal)
{
    values[static_cast<int>(ParamNum)] = FP_FROMFLT(ParamVal);
}
