#include <AnalogSensor.h>

AnalogSensor::AnalogSensor()
{
    filterSize_ = defaultFilterSize();
}

AnalogSensor::AnalogSensor(int input)
{
    input_ = input;
    filterSize_ = defaultFilterSize();
}

AnalogSensor::AnalogSensor(int input, int filterSize)
{
    input_ = input;
    filterSize_ = filterSize;
}

AnalogSensor::AnalogSensor(int input, float lowLimit, float highLimit)
{
    input_ = input;
    filterSize_ = defaultFilterSize();
    lowLimit_ = lowLimit;
    highLimit_ = highLimit;
}

AnalogSensor::AnalogSensor(int input, int filterSize, float lowLimit, float highLimit)
{
    input_ = input;
    filterSize_ = filterSize;
    lowLimit_ = lowLimit;
    highLimit_ = highLimit;
}

int AnalogSensor::defaultFilterSize()
{
    return 20;
}

void AnalogSensor::setFilterSize(int filterSize)
{
    filterSize_ = filterSize;
}

float AnalogSensor::rawHighLimit()
{
    return 65536.0;
}

float AnalogSensor::rawLowLimit()
{
    return 0.0;
}

void AnalogSensor::setInput(int input)
{
    input_ = input;
}

float AnalogSensor::getHighLimit()
{
    return highLimit_;
}

void AnalogSensor::setHighLimit(float hiLimit)
{
    highLimit_ = hiLimit;
}

float AnalogSensor::getLowLimit()
{
    return lowLimit_;
}

void AnalogSensor::setLowLimit(float lowLimit)
{
    lowLimit_ = lowLimit;
}

bool AnalogSensor::isError()
{
    bool errorOver = valueRaw_ > rawHighLimit();
    bool errorBelow = valueRaw_ < rawLowLimit();
    error_ = errorOver || errorBelow;
    return error_;
}

// void AnalogSensor::read()
// {
//     int raw = analogRead(input_);
//     valueRaw_ = (float)raw;

//     scale();
//     filter();
// }

void AnalogSensor::scale()
{
    valueNew_ = (valueRaw_ - rawLowLimit()) / (rawHighLimit() - rawLowLimit()) * (highLimit_ - lowLimit_) + lowLimit_;
}

void AnalogSensor::filter()
{
    valueActual_ = (filterSize_ * valueMemorized_ + valueNew_) / (filterSize_ + 1.0);
    valueMemorized_ = valueActual_;
}

float AnalogSensor::getValue()
{
    return valueActual_;
}

void AnalogSensor::setValueRaw(int raw)
{
    valueRaw_ = (float)raw;

    scale();
    filter();
}