#include "variables.h"
#include <iostream>

using namespace std;



Variables::Variables(){
    intValues = new int[nbIntVar];
    floatValues = new float[nbFloatVar];

    for(int i = 0; i<nbIntVar; ++i){
        intValues[i] = 0;
    }
    for(int i = 0; i<nbFloatVar; ++i){
        floatValues[i] = 0;
    }



}


int Variables::getInt(int index){
    return intValues[index];
}

float Variables::getFloat(int index){
    return floatValues[index];
}