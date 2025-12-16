#include "Inverter.h"
#include <cstdint>
#include <cstdio>

void Inverter::DecodeCanMessage(int id, uint32_t data[2])
{
    switch (id)
    {
    case 0x289:
        printf("inverter_temp");
        break;

    case 0x299:
        printf("motor_temp");
        break;

    case 0x733:
        printf("speed");
        break;
    }
}