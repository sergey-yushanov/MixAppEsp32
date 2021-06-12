#include <Valve.h>

Valve::Valve()
{
    command_ = false;
    commandOpen_ = false;
    commandClose_ = false;
    resetFaulty();
}

// valve faulty
bool Valve::isFaulty()
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

// valve set open command
void Valve::open()
{
    command_ = true;
    commandOpen_ = true;
    commandClose_ = false;
}

// valve set close command
void Valve::close()
{
    command_ = false;
    commandOpen_ = false;
    commandClose_ = true;
}

// valve one-wire circuit command
// todo: need add faulty
bool Valve::isCommand()
{
    return command_;
}

// valve two-wire circuit command
// todo: need add faulty
bool Valve::isCommandOpen()
{
    return commandOpen_;
}

// todo: need add faulty
bool Valve::isCommandClose()
{
    return commandClose_;
}
