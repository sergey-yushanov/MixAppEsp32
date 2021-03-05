#include <Flowmeter.h>

Flowmeter::Flowmeter()
{
    intervalMillis_ = defaultIntervalMillis();
    startMillis_ = millis();
    flowPulseCounter_ = 0;
    volumePulseCounter_ = 0;
}

Flowmeter::Flowmeter(int pin, float pulsesPerLiter)
{
    pin_ = pin;
    pulsesPerLiter_ = pulsesPerLiter;
    intervalMillis_ = defaultIntervalMillis();
    startMillis_ = millis();
    flowPulseCounter_ = 0;
    volumePulseCounter_ = 0;
}

Flowmeter::Flowmeter(int pin, float pulsesPerLiter, long intervalMillis)
{
    pin_ = pin;
    pulsesPerLiter_ = pulsesPerLiter;
    intervalMillis_ = intervalMillis;
    startMillis_ = millis();
    flowPulseCounter_ = 0;
    volumePulseCounter_ = 0;
}

long Flowmeter::defaultIntervalMillis()
{
    return 200;
}

void Flowmeter::setPulsesPerLiter(float pulsesPerLiter)
{
    pulsesPerLiter_ = pulsesPerLiter;
}

void Flowmeter::computeFlow()
{
    passedMillis_ = millis() - startMillis_;
    if (passedMillis_ >= intervalMillis_)
    {
        flow_ = (float)flowPulseCounter_ / (float)pulsesPerLiter_ / (float)passedMillis_ * 60000.0;
        flowPulseCounter_ = 0;
        startMillis_ = millis();
    }
}

void Flowmeter::computeVolume()
{
    volume_ = (double)volumePulseCounter_ / (double)pulsesPerLiter_;
}

void Flowmeter::pulseCounter()
{
    flowPulseCounter_++;
    volumePulseCounter_++;
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
    computeFlow();
    return flow_;
}

double Flowmeter::getVolume()
{
    computeVolume();
    return volume_;
}

void Flowmeter::nullifyVolume()
{
    volumePulseCounter_ = 0;
}