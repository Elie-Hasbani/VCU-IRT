#include "VCU.h"
#include "variables.h"
#include "CanMessageParser.h"
#include <iostream>

VCU vcu;

void CanMessage(CanMessageParser *parser)
{
    Message msg;
    uint32_t arr[2] = {0};
    arr[1] = 0;

    // vcu.receiveCanCallback(msg.id, nullptr, 0);

    parser->readNext(msg);
    arr[0] = static_cast<uint32_t>(msg.value);
    vcu.receiveCanCallback(static_cast<uint32_t>(msg.id), arr, 4);
}

int main()
{
    CanMessageParser parser("/home/ehasbani/IRT/VCU-IRT/data/DataCan.csv");

    vcu.init();
    vcu.Task10ms();

    for (int i = 0; i < 10; ++i)
    {
        CanMessage(&parser);
    }
}
