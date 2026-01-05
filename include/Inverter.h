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
    int32_t inv_temp;
    int32_t speed;
    int32_t voltage;
    int32_t motor_temp;
};

#endif // guard