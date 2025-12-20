#include <cstdint>
#ifndef INVERTER_H
#define INVERTER_H

class Inverter
{
public:
    int getMotorSpeed() { return speed; };
    void setTorque();
    void DecodeCanMessage(int id, uint32_t data[2]);

private:
    int16_t inv_temp;
    int16_t speed;
    int16_t voltage;
    int16_t motor_temp;
};

#endif // guard