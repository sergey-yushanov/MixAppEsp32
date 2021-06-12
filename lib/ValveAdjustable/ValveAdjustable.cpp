#include <ValveAdjustable.h>

ValveAdjustable::ValveAdjustable()
{
    overtime_ = 150;
    limitClose_ = 0.0;
    limitOpen_ = 100.0;
    deadbandClose_ = 1.0;
    deadbandOpen_ = 1.0;
    deadbandPosition_ = 1.0;
    costClose_ = 1.0;
    costOpen_ = 1.0;
}

ValveAdjustable::ValveAdjustable(int overtime, float limitClose, float limitOpen, float deadbandClose, float deadbandOpen, float deadbandPosition, float costClose, float costOpen)
{
    overtime_ = overtime;
    limitClose_ = limitClose;
    limitOpen_ = limitOpen;
    deadbandClose_ = deadbandClose;
    deadbandOpen_ = deadbandOpen;
    deadbandPosition_ = deadbandPosition;
    costClose_ = costClose;
    costOpen_ = costOpen;
}

void ValveAdjustable::calcTimeout(bool _0_1s)
{
    // timeout position Ok
    if (command_ && !positionOk_ && _0_1s)
    {
        time_++;
    }
    if (positionOk_ || faulty_ || !command_)
    {
        time_ = 0;
    }
}

void ValveAdjustable::checkFaulty(bool ack)
{
    // faulty
    if (time_ > overtime_)
    {
        faulty_ = true;
    }
    if (ack)
    {
        faulty_ = false;
    }

    // healthy
    healthy_ = !faulty_;
}

void ValveAdjustable::checkPosition()
{
    // position error
    error_ = setpoint_ - position_;
    errorAbs_ = abs(error_);

    // limit positions
    positionOpen_ = position_ >= limitOpen_ - deadbandOpen_;
    positionClose_ = position_ <= limitClose_ + deadbandClose_;

    // position Ok
    positionOk_ = errorAbs_ <= deadbandPosition_;
}

void ValveAdjustable::checkRequest()
{
    // request open
    if ((errorAbs_ > deadbandPosition_) && (setpoint_ > position_))
    {
        requestOpen_ = true;
        requestClose_ = false;
    }
    if (position_ >= setpoint_ - costOpen_)
    {
        requestOpen_ = false;
    }

    // request close
    if ((errorAbs_ > deadbandPosition_) && (setpoint_ < position_))
    {
        requestClose_ = true;
        requestOpen_ = false;
    }
    if (position_ <= setpoint_ + costClose_)
    {
        requestClose_ = false;
    }
}

void ValveAdjustable::checkCommand()
{
    // commands
    commandOpen = healthy_ && requestOpen_ && !positionOpen_ && !commandClose;
    commandClose = healthy_ && requestClose_ && !positionClose_ && !commandOpen;
    command_ = commandOpen || commandClose;
}

void ValveAdjustable::checkStatus()
{
    status = 0;

    if (command_)
        status = 1;

    if (!positionClose_ && !command_)
        status = 2;

    if (faulty_)
        status = 3;
}

void ValveAdjustable::request()
{
    // if (man)
    // {
    //     sp = spMan;
    // }

    // if (aut)
    // {
    //     sp = spAut;
    // }

    // if (fullyOpen)
    // {
    //     sp = 100.0;
    // }
    // if (fullyClose)
    // {
    //     sp = 0.0;
    // }
}

void ValveAdjustable::checkMode()
{
    if (enable_)
    {
        auto_ = true;
        manual_ = false;
    }

    if (disable_ || faulty_)
    {
        manual_ = true;
        auto_ = false;
    }
}

void ValveAdjustable::resetRequest()
{
    fullyOpen_ = false;
    fullyClose_ = false;
    enable_ = false;
    disable_ = false;
}

void ValveAdjustable::process(bool _0_1s, bool ack)
{
    calcTimeout(_0_1s);
    checkFaulty(ack);
    checkMode();
    request();
    checkPosition();
    checkRequest();
    checkCommand();
    checkStatus();
    resetRequest();
}

void ValveAdjustable::process(float setpoint, float position, bool _0_1s, bool ack)
{
    setpoint_ = setpoint;
    position_ = position;
    process(_0_1s, ack);
}

void ValveAdjustable::setSetpoint(float setpoint)
{
    setpoint_ = setpoint;
}

void ValveAdjustable::setPosition(float position)
{
    position_ = position;
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

void ValveAdjustable::fullOpen()
{
    setPosition(100.0);
}

void ValveAdjustable::fullClose()
{
    setPosition(0.0);
}

void ValveAdjustable::enable()
{
    enable_ = true;
}

void ValveAdjustable::disable()
{
    disable_ = true;
}

float ValveAdjustable::getSetpoint()
{
    return setpoint_;
}

float ValveAdjustable::getPosition()
{
    return position_;
}
