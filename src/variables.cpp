#include "variables.h"
#include <iostream>

using namespace std;

// Define static members
int Variables::intValues[10];
float Variables::floatValues[10];

void Variables::init(){
    for(int i = 0; i < nbIntVar; ++i){
        intValues[i] = 0;
    }
    for(int i = 0; i < nbFloatVar; ++i){
        floatValues[i] = 0;
    }
}

int Variables::getInt(int index){
    return intValues[index];
}

float Variables::getFloat(int index){
    return floatValues[index];
}

void Variables::setInt(int index, int value){
    intValues[index] = value;
}

void Variables::setFloat(int index, float value){
    floatValues[index] = value;
}
