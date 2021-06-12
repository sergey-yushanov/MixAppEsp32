#ifndef VALVE_h
#define VALVE_h

#include <Arduino.h>

class Valve
{
private:
    bool faulty_;
    bool command_;
    bool commandOpen_;
    bool commandClose_;
    int status_;

public:
    Valve();

    void open();
    void close();

    bool getFaulty();
    void resetFaulty();
    int getStatus();

    bool getCommand();
    bool getCommandOpen();
    bool getCommandClose();
};

#endif