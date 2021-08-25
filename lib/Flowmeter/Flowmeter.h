#ifndef FLOWMETER_h
#define FLOWMETER_h

#include <Arduino.h>

class Flowmeter
{
private:
    int pin_;
    volatile float measuredFlow_;
    // float pulsesPerLiter_;
    // long intervalMillis_;
    // long startMillis_;
    // long passedMillis_;
    // volatile long flowPulseCounter_;
    // volatile long volumePulseCounter_;
    // volatile float flow_;
    // volatile double volume_;

    // void computeFlow();
    void computeVolume();

public:
    float pulsesPerLiter_;
    long intervalMillis_;
    long startMillis_;
    long passedMillis_;
    volatile long flowPulseCounter_;
    volatile long volumePulseCounter_;
    volatile float flow_;
    volatile float volume_;

    Flowmeter();
    Flowmeter(int pin, float pulsesPerLiter);
    Flowmeter(int pin, float pulsesPerLiter, long intervalMillis);
    long defaultIntervalMillis();

    float getPulsesPerLiter();
    void setPulsesPerLiter(float pulsesPerLiter);

    void pulseCounter();

    int getPin();
    void setPin(int pin);

    void computeFlow();
    float getFlow();
    float getVolume();

    void nullifyVolume();
};

#endif