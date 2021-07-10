#include <AnalogSensor.h>

AnalogSensor::AnalogSensor()
{
    setFilterSize();
    setRawLowLimit();
    setRawHighLimit();
    setValueLowLimit();
    setValueHighLimit();
}

float AnalogSensor::getRawLowLimit()
{
    return rawLowLimit_;
}

void AnalogSensor::setRawLowLimit(float rawLowLimit)
{
    rawLowLimit_ = rawLowLimit;
}

float AnalogSensor::getRawHighLimit()
{
    return rawHighLimit_;
}

void AnalogSensor::setRawHighLimit(float rawHighLimit)
{
    rawHighLimit_ = rawHighLimit;
}

float AnalogSensor::getValueLowLimit()
{
    return valueLowLimit_;
}

void AnalogSensor::setValueLowLimit(float valueLowLimit)
{
    valueLowLimit_ = valueLowLimit;
}

float AnalogSensor::getValueHighLimit()
{
    return valueHighLimit_;
}

void AnalogSensor::setValueHighLimit(float valueHighLimit)
{
    valueHighLimit_ = valueHighLimit;
}

void AnalogSensor::setFilterSize(int filterSize)
{
    filterSize_ = filterSize;
}

float AnalogSensor::getRaw()
{
    return raw_;
}

void AnalogSensor::setRaw(float raw)
{
    raw_ = raw;

    scale();
    filter();
}

void AnalogSensor::setIntRaw(int intRaw)
{
    setRaw((float)intRaw);
}

bool AnalogSensor::isError()
{
    bool errorOver = raw_ > rawHighLimit_;
    bool errorBelow = raw_ < rawLowLimit_;
    return errorOver || errorBelow;
}

float AnalogSensor::getValue()
{
    return value_;
}

void AnalogSensor::scale()
{
    if (isError())
        valueNew_ = 0.0;
    else
        valueNew_ = (raw_ - rawLowLimit_) / (rawHighLimit_ - rawLowLimit_) * (valueHighLimit_ - valueLowLimit_) + valueLowLimit_;
}

void AnalogSensor::filter()
{
    value_ = (filterSize_ * valueMemorized_ + valueNew_) / (filterSize_ + 1.0);
    valueMemorized_ = value_;
}
