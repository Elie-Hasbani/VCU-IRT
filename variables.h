
#ifndef VARIABLES_H
#define VARIABLES_H
enum Op_Mode {MODE_RUN, MODE_CHARGE};
enum Pot_Mode {MODE_ONE_POT, MODE_TWO_POT};



//ints
#define SPEED 0
#define DIRECTION 1
#define BRAKE 2
#define OP_MODE 3
#define POT_MODE 4
#define DERATE_REASON 5





//floats
#define POT1 0
#define POT2 1
#define INVERTER_TEMP 2
#define MOTOR_TEMP 3
#define INVERTER_TEMP_MAX 4
#define MOTOR_TEMP_MAX 5
#define POTNOM 6


class Variables
{
    friend class VCU;
    friend class utils;
    friend class Throttle;

    public:
    int getInt(int index);
    float getFloat(int index);

    private:
    void setInt(int index, int value){intValues[index] = value;};
    void setFloat(int index, float value){floatValues[index] = value;};
    Variables();
    int * intValues;
    float * floatValues;
    int nbIntVar = 10;
    int nbFloatVar = 10;

};

#endif


