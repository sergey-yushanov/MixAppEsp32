#ifndef FLOWMETER_h
#define FLOWMETER_h

#include <Arduino.h>

class Flowmeter
{
private:
    int pin_;
    volatile float measuredFlow_;
    volatile float memoryFlow_;
    volatile float filterSize_ = 10.0;

    float limitFlow_ = 145.0;
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
    long intervalMicros_;
    long startMicros_;
    long passedMicros_;
    volatile long flowPulseCounter_;
    volatile long volumePulseCounter_;
    volatile float flow_;
    volatile float volume_;

    long risingIntervalMicros;
    long risingStartMicros;
    // bool risingAccess = true;
    bool risingReady = true;
    int risingLowCount = 0;

    portMUX_TYPE flowMux = portMUX_INITIALIZER_UNLOCKED;

    Flowmeter();
    Flowmeter(int pin, float pulsesPerLiter);
    Flowmeter(int pin, float pulsesPerLiter, long intervalMillis);
    long defaultIntervalMicros();

    float getPulsesPerLiter();
    void setPulsesPerLiter(float pulsesPerLiter);

    void pulseCounter();

    int getPin();
    void setPin(int pin);

    void computeFlow();
    float getFlow();
    float getVolume();

    void nullifyVolume();

    void setLimitFlow(float limitFlow);
    float getLimitFlow();
};

#endif