#include "plant.h"

Collector collector;
SingleDos singleDos;
Flowmeter m1;
ValveAdjustable valveAdjustable;

OnTimer pumpStartDelay_;

float carrierRequiredVolume;
float carrierDosedVolume;

bool ack;
bool showSettings;

bool pumpCommand;
// const int pumpPin = 5;

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
float carrierDosedPercent = 0;

bool loopStart_;
bool loopValveOk_;
bool loopPump_;
bool loopCollector_;
bool loopSingleDos_;
bool loopRunning_;
bool loopDone_;
bool loopWashing_;

// SimpleKalmanFilter kalman34(2, 2, 0.01);
// SimpleKalmanFilter kalman35(2, 2, 0.01);

// common flowmeter
void IRAM_ATTR m1Pulse()
{
    portENTER_CRITICAL_ISR(&m1.flowMux);
    m1.pulseCounter();
    portEXIT_CRITICAL_ISR(&m1.flowMux);
}

void m1Setup()
{
    m1.setPin(4);
    m1.setPulsesPerLiter(50); //(100.0); // 50.0
    m1.risingStartMicros = micros();
    m1.risingIntervalMicros = 800;
    m1.risingReady = true;
    pinMode(m1.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m1.getPin()), m1Pulse, RISING);
}

// dispenser collector flowmeter
void IRAM_ATTR g1Pulse()
{
    portENTER_CRITICAL_ISR(&collector.flowmeter.flowMux);
    collector.flowmeter.pulseCounter();
    portEXIT_CRITICAL_ISR(&collector.flowmeter.flowMux);
}

void g1Setup()
{
    collector.flowmeter.setPin(2);
    collector.flowmeter.setPulsesPerLiter(106.777); //(106.777);
    collector.flowmeter.risingStartMicros = micros();
    collector.flowmeter.risingIntervalMicros = 3700;
    collector.flowmeter.risingReady = true;
    pinMode(collector.flowmeter.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(collector.flowmeter.getPin()), g1Pulse, RISING);
}

// dispenser single flowmeter
void IRAM_ATTR g2Pulse()
{
    portENTER_CRITICAL_ISR(&singleDos.flowmeter.flowMux);
    singleDos.flowmeter.pulseCounter();
    portEXIT_CRITICAL_ISR(&singleDos.flowmeter.flowMux);
}

void g2Setup()
{
    singleDos.flowmeter.setPin(0);
    singleDos.flowmeter.setPulsesPerLiter(106.585); //(106.777);
    singleDos.flowmeter.risingStartMicros = micros();
    singleDos.flowmeter.risingIntervalMicros = 3700;
    singleDos.flowmeter.risingReady = true;
    pinMode(singleDos.flowmeter.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(singleDos.flowmeter.getPin()), g2Pulse, RISING);
}

// timeouts
void incTimeouts()
{
    // valveAdjustable.incTimeout();
    // collector.valveAdjustable.incTimeout();
    // singleDos.valveAdjustable.incTimeout();
}

void incTimers()
{
    pumpStartDelay_.inc100msTimer();
    collector.incTimers();
    singleDos.incTimers();
}

// reset faults for all equipment
void resetFaults()
{
    valveAdjustable.resetFaulty();
    collector.valveAdjustable.resetFaulty();
    singleDos.valveAdjustable.resetFaulty();
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
    g2Setup();

    // analogSetClockDiv(20);
    // analogSetWidth(11);

    // pinMode(pumpPin, OUTPUT);
    pumpCommand = false;

    valveAdjustable.setDeadbandClose(8.0);
    valveAdjustable.setDeadbandOpen(8.0);
    valveAdjustable.setDeadbandPosition(8.0);
    valveAdjustable.setCostClose(8.0);
    valveAdjustable.setCostOpen(8.0);

    for (int i = 0; i < collector.nValves_ - 1; i++)
    {
        collector.valveNums[i] = 0;
    }
    collector.order = 0;
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
    singleDos.flowmeter.computeFlow();
    m1.computeFlow();
}

void flowReady()
{
    // m1.risingReady = true;

    if (!digitalRead(m1.getPin()))
    {
        m1.risingLowCount++;
        if (m1.risingLowCount > 1)
        {
            m1.risingLowCount = 0;
            m1.risingReady = true;
        }
    }

    if (!digitalRead(collector.flowmeter.getPin()))
    {
        collector.flowmeter.risingLowCount++;
        if (collector.flowmeter.risingLowCount > 4)
        {
            collector.flowmeter.risingLowCount = 0;
            collector.flowmeter.risingReady = true;
        }
    }

    if (!digitalRead(singleDos.flowmeter.getPin()))
    {
        singleDos.flowmeter.risingLowCount++;
        if (singleDos.flowmeter.risingLowCount > 4)
        {
            singleDos.flowmeter.risingLowCount = 0;
            singleDos.flowmeter.risingReady = true;
        }
    }
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
    singleDos.loop();
    commonLoop();

    pumpStartDelay_.on100msTimer(loopValveOk_, 30);

    carrierDosedVolume = m1.getVolume();

    if (loopRunning_)
    {
        // carrierDosedVolume = m1.getVolume();
        // прекращаем дозацию компонентов, если осталось 20% носителя, продолжает дозироваться носитель
        carrierDosedPercent = (carrierRequiredVolume - carrierDosedVolume) / carrierRequiredVolume * 100;
    }

    // 1. command to open valve to setpoint
    if (loopStart_)
    {
        loopStart_ = false;
        valveAdjustable.setSetpoint(valveSetpoint);
        Serial.println("Loop Start!");
        return;
    }

    // 2. wait valve setpoint
    if (loopRunning_ && !loopValveOk_)
    {
        // loopValveOk_ = valveAdjustable.isPositionOk();
        loopValveOk_ = true;
        return;
    }

    // 3. start pump and collector
    if (loopRunning_ && loopValveOk_ && !loopPump_)
    {
        // Serial.println("Loop valve setpoint: ");

        m1.nullifyVolume();
        if (pumpStartDelay_.status)
        {
            pumpCommand = true;
            loopPump_ = true;
        }
        return;
    }

    // 3.1 start collector
    if (loopRunning_ && loopValveOk_ && loopPump_ && !loopCollector_ && collector.loopUsing_)
    {
        collector.loopStart();
        loopCollector_ = true;
        Serial.println("Loop collector Start!");
    }

    // 3.2 start single dos
    if (loopRunning_ && loopValveOk_ && loopPump_ && !loopSingleDos_ && singleDos.loopUsing_)
    {
        singleDos.loopStart();
        loopSingleDos_ = true;
        Serial.println("Loop single dos Start!");
    }

    // 4. need washing
    if (loopRunning_ && loopValveOk_ && loopPump_ && !loopDone_ && !loopWashing_)
    {
        if (collector.loopUsing_ && (abs(carrierDosedPercent) < carrierReserve))
        {
            Serial.println("abs(carrierDosedPercent) < carrierReserve");
            loopWashing_ = true;
            loopCollectorWashingStart();
            // return;
        }
    }

    // 5. dosing done
    // if (loopRunning_ && loopValveOk_ && loopPump_ && !loopDone_)
    // {
    // todo: прибавить отдозированные компоненты!!!
    loopDone_ = loopRunning_ && (carrierDosedVolume >= carrierRequiredVolume);
    //     return;
    // }

    // 6. loop done
    if (loopDone_)
    {
        if (collector.loopUsing_)
            loopCollectorWashingStop();
        // if (collector.valveAdjustable.isClosed())
        // {
        Serial.println("Loop Done!");
        loopStop();
        // }
    }
}

void loopStart()
{
    loopStart_ = true;
    loopValveOk_ = false;
    loopPump_ = false;
    loopCollector_ = false;
    loopSingleDos_ = false;
    loopRunning_ = true;
    loopDone_ = false;

    loopWashing_ = false;
    loopCollectorWashingStop();

    // m1.nullifyVolume();
    // collector.loopStart();
}

void loopDevicesStop()
{
    collector.loopStop();
    singleDos.loopStop();
}

void loopCollectorWashingStart()
{
    collector.loopStop();
    collector.washingCarrierReserve_ = true;
    collector.washingStart_ = true;
}

void loopCollectorWashingStop()
{
    collector.washingCarrierReserve_ = false;
    collector.resetWash();
    collector.closeAll();
}

void loopStop()
{
    loopCollectorWashingStop();
    loopDevicesStop();

    pumpCommand = false;
    valveAdjustable.fullyClose();

    loopStart_ = false;
    loopValveOk_ = false;
    loopPump_ = false;
    loopCollector_ = false;
    loopSingleDos_ = false;
    loopRunning_ = false;
    loopDone_ = false;

    loopWashing_ = false;
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