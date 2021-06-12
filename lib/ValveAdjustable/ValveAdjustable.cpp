#include <ValveAdjustable.h>

ValveAdjustable::ValveAdjustable()
{
    setOvertime();
    setLimits();
    setDeadbands();
    setCosts();
}

void ValveAdjustable::incTimeout()
{
    // timeout position with clk 0.1 sec
    if (command_ && !positionOk_)
    {
        time_++;
    }
    if (positionOk_ || faulty_ || !command_)
    {
        time_ = 0;
    }

    timeout_ = (time_ > overtime_);
    setFaulty();
}

int ValveAdjustable::getTime()
{
    return time_;
}

void ValveAdjustable::setFaulty()
{
    if (timeout_)
        faulty_ = true;
}

bool ValveAdjustable::isFaulty()
{
    return faulty_;
}

void ValveAdjustable::resetFaulty()
{
    faulty_ = false;
    time_ = 0;
}

int ValveAdjustable::getStatus()
{
    status_ = 0;

    if (command_)
        status_ = 1;

    if (!positionClose_ && !command_)
        status_ = 2;

    if (faulty_)
        status_ = 3;

    return status_;
}

void ValveAdjustable::setRequest()
{
    // position error
    error_ = setpoint_ - position_;
    errorAbs_ = abs(error_);

    // limit positions
    positionOpen_ = position_ >= limitOpen_ - deadbandOpen_;
    positionClose_ = position_ <= limitClose_ + deadbandClose_;

    // position Ok
    positionOk_ = errorAbs_ <= deadbandPosition_;

    // request open
    if ((errorAbs_ > deadbandPosition_) && (setpoint_ > position_))
    {
        requestOpen_ = true;
        requestClose_ = false;
    }
    if ((position_ >= setpoint_ - costOpen_) || positionOpen_)
    {
        requestOpen_ = false;
    }

    // request close
    if ((errorAbs_ > deadbandPosition_) && (setpoint_ < position_))
    {
        requestClose_ = true;
        requestOpen_ = false;
    }
    if ((position_ <= setpoint_ + costClose_) || positionClose_)
    {
        requestClose_ = false;
    }

    // requests reset with faulty
    if (faulty_)
    {
        requestOpen_ = false;
        requestClose_ = false;
    }
}

void ValveAdjustable::setCommand()
{
    setRequest();

    commandOpen_ = !faulty_ && requestOpen_ && !positionOpen_ && !commandClose_;
    commandClose_ = !faulty_ && requestClose_ && !positionClose_ && !commandOpen_;
    command_ = commandOpen_ || commandClose_;
}

bool ValveAdjustable::isCommandOpen()
{
    return commandOpen_;
}

bool ValveAdjustable::isCommandClose()
{
    return commandClose_;
}

float ValveAdjustable::getSetpoint()
{
    return setpoint_;
}

void ValveAdjustable::setSetpoint(float setpoint)
{
    setpoint_ = setpoint;
    setCommand();
}

float ValveAdjustable::getPosition()
{
    return position_;
}

void ValveAdjustable::setPosition(float position)
{
    position_ = position;
    setCommand();
}

void ValveAdjustable::setOvertime(int overtime)
{
    overtime_ = overtime;
}

void ValveAdjustable::setLimits(float limitClose, float limitOpen)
{
    limitClose_ = limitClose;
    limitOpen_ = limitOpen;
}

void ValveAdjustable::setDeadbands(float deadbandClose, float deadbandOpen, float deadbandPosition)
{
    deadbandClose_ = deadbandClose;
    deadbandOpen_ = deadbandOpen;
    deadbandPosition_ = deadbandPosition;
}

void ValveAdjustable::setCosts(float costClose, float costOpen)
{
    costClose_ = costClose;
    costOpen_ = costOpen;
}

void ValveAdjustable::fullyOpen()
{
    setSetpoint(100.0);
}

void ValveAdjustable::fullyClose()
{
    setSetpoint(0.0);
}
