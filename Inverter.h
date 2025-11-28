#include <cstdint>
#ifndef INVERTER_H
#define INVERTER_H

class Inverter
{
public:
    int getMotorSpeed() { return 1000; };
    void setTorque();
    void DecodeCanMessage(int id, uint32_t *data);

private:
    int16_t inv_temp;
    int16_t speed;
    int16_t voltage;
    int16_t motor_temp;
};

#endif // guard