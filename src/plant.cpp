#include "plant.h"

// #define ADC_PIN_34 34
// #define ADC_PIN_35 35
// #define voltage_divider_offset 2.174 // Should be a value of 2.000, but ADC input impedance loads the voltage divider, requiring a correction

Collector collector;
Flowmeter m1;
ValveAdjustable valveAdjustable;
float carrierRequiredVolume;
float carrierDosedVolume;

bool ack;
bool showSettings;

bool pumpCommand;
const int pumpPin = 5;

bool isLoopRunning;

long openTime;
long openTimeBegin;
long openTimeEnd;
bool openTimeCalc;
float openPosition;

long closeTime;
long closeTimeBegin;
long closeTimeEnd;
bool closeTimeCalc;

// loop parameters
float ratioVolume = 1.1;
float ratioVolumeMicro = 1.1;
float valveSetpoint = 60.0;
float carrierReserve = 20.0;

bool loopStart_;
bool loopValveOk_;
bool loopCollector_;
bool loopRunning_;
bool loopDone_;

// SimpleKalmanFilter kalman34(2, 2, 0.01);
// SimpleKalmanFilter kalman35(2, 2, 0.01);

// dispenser collector flowmeter
void g1Setup()
{
    collector.flowmeter.setPin(19);
    collector.flowmeter.setPulsesPerLiter(106.777);
    pinMode(collector.flowmeter.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(collector.flowmeter.getPin()), g1Pulse, FALLING);
}

void IRAM_ATTR g1Pulse()
{
    collector.flowmeter.pulseCounter();
}

// common flowmeter
void m1Setup()
{
    m1.setPin(18);
    m1.setPulsesPerLiter(100.0); // 50.0
    pinMode(m1.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m1.getPin()), m1Pulse, FALLING);
}

void IRAM_ATTR m1Pulse()
{
    m1.pulseCounter();
}

// timeouts
void incTimeouts()
{
    valveAdjustable.incTimeout();
    collector.valveAdjustable.incTimeout();
}

void incTimers()
{
    collector.incTimers();
}

// reset faults for all equipment
void resetFaults()
{
    valveAdjustable.resetFaulty();
    collector.valveAdjustable.resetFaulty();
    for (int i = 0; i < collector.nValves_; i++)
    {
        collector.valves[i].resetFaulty();
    }
}

void plantSetup()
{
    ack = true;
    showSettings = true;
    isLoopRunning = false;

    // dataSetup();

    m1Setup();
    g1Setup();

    // analogSetClockDiv(20);
    // analogSetWidth(11);

    // pinMode(pumpPin, OUTPUT);
    pumpCommand = false;
}

void plantLoop()
{
    if (ack)
    {
        resetFaults();
        ack = false;
    }

    mixLoop();

    // float measured_value34 = (float)analogRead(ADC_PIN_34);
    // float estimated_value34 = kalman34.updateEstimate(measured_value34);

    // valveAdjustable.getPositionSensor()->setIntRaw(value);

    // valveAdjustable.getPositionSensor()->setRaw(measured_value34);
    // valveAdjustable.updatePosition();

    // value = analogRead(35);
    // collector.valveAdjustable.getPositionSensor()->setIntRaw(value);
    // collector.valveAdjustable.updatePosition();

    // calculate open/close time
    // if (collector.valveAdjustable.isOpened() && openTimeCalc)
    // {
    //     openTimeEnd = millis();
    //     openTime = openTimeEnd - openTimeBegin;
    //     openPosition = collector.valveAdjustable.getPositionSensor()->getRaw();
    //     openTimeCalc = false;
    // }

    // if (collector.valveAdjustable.isClosed() && closeTimeCalc)
    // {
    //     closeTimeEnd = millis();
    //     closeTime = closeTimeEnd - closeTimeBegin;
    //     closeTimeCalc = false;
    // }
}

void flowLoop()
{
    collector.flowmeter.computeFlow();
    m1.computeFlow();
}

void commonLoop()
{
    // pump command set/reset pin
    // if (pumpCommand)
    // {
    //     digitalWrite(pumpPin, HIGH);
    // }
    // else
    // {
    //     digitalWrite(pumpPin, LOW);
    // }
}

void mixLoop()
{
    collector.loop();
    commonLoop();

    // 1. command to open valve to setpoint
    if (loopStart_)
    {
        loopStart_ = false;
        valveAdjustable.setSetpoint(valveSetpoint);
    }

    // 2. wait valve setpoint
    if (loopRunning_ && !loopValveOk_)
    {
        loopValveOk_ = valveAdjustable.isPositionOk();
    }

    // 3. start pump and collector
    if (loopRunning_ && loopValveOk_ && !loopCollector_)
    {
        m1.nullifyVolume();
        pumpCommand = true;
        collector.loopStart();
        loopCollector_ = true;
    }

    // 4. dosing done
    if (loopRunning_ && loopValveOk_ && loopCollector_)
    {
        carrierDosedVolume = m1.getVolume();
        loopDone_ = collector.loopDone_ || (carrierDosedVolume > carrierRequiredVolume);
    }

    // 5. loop done
    if (loopDone_)
    {
        loopStop();
    }
}

void loopStart()
{
    loopStart_ = true;
    loopValveOk_ = false;
    loopCollector_ = false;
    loopRunning_ = true;
    loopDone_ = false;

    // m1.nullifyVolume();
    // collector.loopStart();
}

void loopStop()
{
    collector.loopStop();

    pumpCommand = false;
    valveAdjustable.fullyClose();

    loopStart_ = false;
    loopValveOk_ = false;
    loopCollector_ = false;
    loopRunning_ = false;
    loopDone_ = false;
}

void loopPause()
{
    collector.loopPause();
}

void pumpStart()
{
    pumpCommand = true;
}

void pumpStop()
{
    pumpCommand = false;
}