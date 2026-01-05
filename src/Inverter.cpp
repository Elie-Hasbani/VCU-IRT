#include "Inverter.h"
#include <cstdint>
#include <cstdio>

void Inverter::DecodeCanMessage(int id, uint32_t data[2])
{
    switch (id)
    {
    case 1:
        printf("inverter_temp : %d\n", data[0]);
        Inverter::inv_temp = data[0];
        break;

    case 2:
        printf("motor_temp : %d\n", data[0]);
        Inverter::motor_temp = data[0];
        break;

    case 3:
        printf("voltage: %d\n", data[0]);
        Inverter::voltage = data[0];
        break;
    case 4:
        printf("motor_speed: %d\n", data[0]);
        Inverter::speed = data[0];
        break;
    }
}