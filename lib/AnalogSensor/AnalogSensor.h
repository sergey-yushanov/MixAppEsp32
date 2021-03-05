#ifndef ANALOG_SENSOR_h
#define ANALOG_SENSOR_h

#include <Arduino.h>

class AnalogSensor
{
private:
    int pin_;
    int filterSize_;
    float valueRaw_;
    float valueNew_;
    float valueActual_;
    float valueMemorized_;
    float highLimit_;
    float lowLimit_;

    bool error_;

    void scale();
    void filter();

public:
    AnalogSensor();
    AnalogSensor(int pin);
    AnalogSensor(int pin, int filterSize);
    AnalogSensor(int pin, float lowLimit, float highLimit);
    AnalogSensor(int pin, int filterSize, float lowLimit, float highLimit);

    float rawHighLimit();
    float rawLowLimit();

    void setPin(int pin);

    float getHighLimit();
    void setHighLimit(float hiLimit);

    float getLowLimit();
    void setLowLimit(float lowLimit);

    int defaultFilterSize();
    void setFilterSize(int filterSize);

    bool isError();
    void read();
    float getValue();
};

#endif