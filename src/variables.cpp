#include "variables.h"
#include "my_fp.h"
#include <iostream>

using namespace std;

s32fp Variables::values[100];

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
