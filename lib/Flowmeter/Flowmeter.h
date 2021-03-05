#ifndef FLOWMETER_h
#define FLOWMETER_h

#include <Arduino.h>

class Flowmeter
{
private:
    int pin_;
    float pulsesPerLiter_;
    long intervalMillis_;
    long startMillis_;
    long passedMillis_;
    volatile long flowPulseCounter_;
    volatile long volumePulseCounter_;
    volatile float flow_;
    volatile double volume_;

    void computeFlow();
    void computeVolume();

public:
    Flowmeter();
    Flowmeter(int pin, float pulsesPerLiter);
    Flowmeter(int pin, float pulsesPerLiter, long intervalMillis);
    long defaultIntervalMillis();
    void setPulsesPerLiter(float pulsesPerLiter);
    void pulseCounter();
    int getPin();
    void setPin(int pin);
    float getFlow();
    double getVolume();
    void nullifyVolume();
};

#endif