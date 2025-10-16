#include "VCU.h"
#include "variables.h"
#include <iostream>

using namespace std;



void VCU::Task10ms(){
    cout<<"ok0\n";
    Variables * variables = new Variables;
    cout<< "ok1\n";
    variables->setInt(SPEED, 5);

    cout<< (variables -> getInt(SPEED))<<"\n";



}