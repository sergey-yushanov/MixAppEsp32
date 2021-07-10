#ifndef ANALOG_SENSOR_h
#define ANALOG_SENSOR_h

#include <Arduino.h>

class AnalogSensor
{
private:
    int filterSize_;

    float raw_;
    float rawLowLimit_;
    float rawHighLimit_;

    float value_;
    float valueLowLimit_;
    float valueHighLimit_;

    float valueNew_;
    float valueMemorized_;

    void scale();
    void filter();

public:
    AnalogSensor();

    float getRawLowLimit();
    void setRawLowLimit(float rawLowLimit = 0.0);

    float getRawHighLimit();
    void setRawHighLimit(float rawHighLimit = 65536.0);

    float getValueLowLimit();
    void setValueLowLimit(float valueLowLimit = 0.0);

    float getValueHighLimit();
    void setValueHighLimit(float valueHighLimit = 100.0);

    void setFilterSize(int filterSize = 0);

    float getRaw();
    void setRaw(float raw);
    void setIntRaw(int intRaw);

    bool isError();
    float getValue();
};

#endif