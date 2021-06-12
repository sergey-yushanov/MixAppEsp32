#include <DispenserCollector.h>

void DispenserCollector::setup()
{
}

void DispenserCollector::loop()
{
    for (int i = 0; i < nValves_; i++)
    {
        valves[i].loop(true);
    }

    //valve
}