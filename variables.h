
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




//floats
#define POT1 0
#define POT2 1


class Variables
{
    friend class VCU;
    public:
    int getInt(int index);
    float getFloat(int index);

    private:
    void setInt(int index, int value){intValues[index] = value;};
    void setFloat(int index, float value){floatValues[index] = value;};
    Variables();
    int * intValues;
    float * floatValues;
    int nbIntVar = 5;
    int nbFloatVar = 5;

};

#endif


