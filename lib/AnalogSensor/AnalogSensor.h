#ifndef ANALOG_SENSOR_h
#define ANALOG_SENSOR_h

#include <Arduino.h>

class AnalogSensor
{
private:
    int input_;
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
    AnalogSensor(int input);
    AnalogSensor(int input, int filterSize);
    AnalogSensor(int input, float lowLimit, float highLimit);
    AnalogSensor(int input, int filterSize, float lowLimit, float highLimit);

    float rawHighLimit();
    float rawLowLimit();

    void setInput(int input);

    float getHighLimit();
    void setHighLimit(float hiLimit);

    float getLowLimit();
    void setLowLimit(float lowLimit);

    int defaultFilterSize();
    void setFilterSize(int filterSize);

    bool isError();
    //void read();
    float getValue();

    void setValueRaw(int raw);
};

#endif