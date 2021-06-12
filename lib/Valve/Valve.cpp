#include <Valve.h>

Valve::Valve()
{
    command_ = false;
    commandOpen_ = false;
    commandClose_ = false;
    resetFaulty();
}

// valve faulty
bool Valve::getFaulty()
{
    return faulty_;
}

void Valve::resetFaulty()
{
    faulty_ = false;
}

// valve status
int Valve::getStatus()
{
    status_ = 0;
    if (command_)
        status_ = 1;
    if (faulty_)
        status_ = 20;
    return status_;
}

// valve open command
void Valve::open()
{
    command_ = true;
    commandOpen_ = true;
    commandClose_ = false;
}

// valve close command
void Valve::close()
{
    command_ = false;
    commandOpen_ = false;
    commandClose_ = true;
}

// valve one-wire circuit command
bool Valve::getCommand()
{
    return command_;
}

// valve two-wire circuit command
bool Valve::getCommandOpen()
{
    return commandOpen_;
}

bool Valve::getCommandClose()
{
    return commandClose_;
}