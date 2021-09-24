#include <Flowmeter.h>

Flowmeter::Flowmeter()
{
    intervalMicros_ = defaultIntervalMicros();
    startMicros_ = micros();
    flowPulseCounter_ = 0;
    volumePulseCounter_ = 0;
}

Flowmeter::Flowmeter(int pin, float pulsesPerLiter)
{
    pin_ = pin;
    pulsesPerLiter_ = pulsesPerLiter;
    intervalMicros_ = defaultIntervalMicros();
    startMicros_ = micros();
    flowPulseCounter_ = 0;
    volumePulseCounter_ = 0;
}

Flowmeter::Flowmeter(int pin, float pulsesPerLiter, long intervalMillis)
{
    pin_ = pin;
    pulsesPerLiter_ = pulsesPerLiter;
    intervalMicros_ = intervalMillis;
    startMicros_ = micros();
    flowPulseCounter_ = 0;
    volumePulseCounter_ = 0;
}

long Flowmeter::defaultIntervalMicros()
{
    return 200;
}

float Flowmeter::getPulsesPerLiter()
{
    return pulsesPerLiter_;
}

void Flowmeter::setPulsesPerLiter(float pulsesPerLiter)
{
    pulsesPerLiter_ = pulsesPerLiter;
}

void Flowmeter::computeFlow()
{
    passedMicros_ = micros() - startMicros_;

    if (passedMicros_ >= intervalMicros_)
    {
        measuredFlow_ = (float)flowPulseCounter_ / (float)pulsesPerLiter_ / (float)passedMicros_ * 60000.0 * 1000.0;

        // filter for flow
        flow_ = (measuredFlow_ + (filterSize_ - 1) * memoryFlow_) / filterSize_;
        memoryFlow_ = flow_;

        flowPulseCounter_ = 0;
        startMicros_ = micros();
    }
}

void Flowmeter::computeVolume()
{
    volume_ = (double)volumePulseCounter_ / (double)pulsesPerLiter_;
}

void Flowmeter::pulseCounter()
{
    if (risingReady && (micros() - risingStartMicros) >= risingIntervalMicros)
    {
        risingReady = false;
        flowPulseCounter_++;
        volumePulseCounter_++;
        risingStartMicros = micros();
    }
}

int Flowmeter::getPin()
{
    return pin_;
}

void Flowmeter::setPin(int pin)
{
    pin_ = pin;
}

float Flowmeter::getFlow()
{
    // computeFlow();
    return flow_;
}

float Flowmeter::getVolume()
{
    computeVolume();
    return volume_;
}

void Flowmeter::nullifyVolume()
{
    volumePulseCounter_ = 0;
}

void Flowmeter::setLimitFlow(float limitFlow)
{
    limitFlow_ = limitFlow;
}

float Flowmeter::getLimitFlow()
{
    return limitFlow_;
}