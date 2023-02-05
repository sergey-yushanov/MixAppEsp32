#include "plant.h"

Collector collectors[nCollectors];
// SingleDos singleDos;
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
// bool loopSingleDos_;
bool loopRunning_;
bool loopDone_;
bool loopWashing_;

// flow meters
float banjoDefaultPulsesPerLiter = 50.0;
float darkontDefaultPulsesPerLiter = 106.777;

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
    // m1.setPulsesPerLiter(m1PulsesPerLiter); //(100.0); // 50.0
    m1.risingStartMicros = micros();
    m1.risingIntervalMicros = 800;
    m1.risingReady = true;
    pinMode(m1.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m1.getPin()), m1Pulse, RISING);
}

// dispenser collector flowmeter
void IRAM_ATTR g1Pulse()
{
    portENTER_CRITICAL_ISR(&collectors[0].flowmeter.flowMux);
    collectors[0].flowmeter.pulseCounter();
    portEXIT_CRITICAL_ISR(&collectors[0].flowmeter.flowMux);
}

void g1Setup()
{
    collectors[0].flowmeter.setPin(2);
    // collector.flowmeter.setPulsesPerLiter(g1PulsesPerLiter); //(106.777);
    collectors[0].flowmeter.risingStartMicros = micros();
    collectors[0].flowmeter.risingIntervalMicros = 3700;
    collectors[0].flowmeter.risingReady = true;
    pinMode(collectors[0].flowmeter.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(collectors[0].flowmeter.getPin()), g1Pulse, RISING);
}

// dispenser collector flowmeter
void IRAM_ATTR g2Pulse()
{
    portENTER_CRITICAL_ISR(&collectors[1].flowmeter.flowMux);
    collectors[1].flowmeter.pulseCounter();
    portEXIT_CRITICAL_ISR(&collectors[1].flowmeter.flowMux);
}

void g2Setup()
{
    collectors[1].flowmeter.setPin(0);
    // collector.flowmeter.setPulsesPerLiter(g1PulsesPerLiter); //(106.777);
    collectors[1].flowmeter.risingStartMicros = micros();
    collectors[1].flowmeter.risingIntervalMicros = 3700;
    collectors[1].flowmeter.risingReady = true;
    pinMode(collectors[1].flowmeter.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(collectors[1].flowmeter.getPin()), g2Pulse, RISING);
}

// dispenser single flowmeter
// void IRAM_ATTR g2Pulse()
// {
//     portENTER_CRITICAL_ISR(&singleDos.flowmeter.flowMux);
//     singleDos.flowmeter.pulseCounter();
//     portEXIT_CRITICAL_ISR(&singleDos.flowmeter.flowMux);
// }

// void g2Setup()
// {
//     singleDos.flowmeter.setPin(0);
//     // singleDos.flowmeter.setPulsesPerLiter(g2PulsesPerLiter); //(106.777);
//     singleDos.flowmeter.risingStartMicros = micros();
//     singleDos.flowmeter.risingIntervalMicros = 3700;
//     singleDos.flowmeter.risingReady = true;
//     pinMode(singleDos.flowmeter.getPin(), INPUT_PULLUP);
//     attachInterrupt(digitalPinToInterrupt(singleDos.flowmeter.getPin()), g2Pulse, RISING);
// }

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
    for (int i = 0; i < nCollectors; i++)
    {
        collectors[i].incTimers();
    }
    // singleDos.incTimers();
}

// reset faults for all equipment
void resetFaults()
{
    valveAdjustable.resetFaulty();
    for (int i = 0; i < nCollectors; i++)
    {
        collectors[i].valveAdjustable.resetFaulty();
        for (int j = 0; j < collectors[i].nValves_; j++)
        {
            collectors[i].valves[j].resetFaulty();
        }
    }
    // singleDos.valveAdjustable.resetFaulty();
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

    for (int i = 0; i < nCollectors; i++)
    {
        for (int j = 0; j < collectors[i].nValves_ - 1; j++)
        {
            collectors[i].valveNums[j] = 0;
        }
        collectors[i].order = 0;
    }
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
    for (int i = 0; i < nCollectors; i++)
    {
        collectors[i].flowmeter.computeFlow();
    }
    // singleDos.flowmeter.computeFlow();
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

    for (int i = 0; i < nCollectors; i++)
    {
        if (!digitalRead(collectors[i].flowmeter.getPin()))
        {
            collectors[i].flowmeter.risingLowCount++;
            if (collectors[i].flowmeter.risingLowCount > 4)
            {
                collectors[i].flowmeter.risingLowCount = 0;
                collectors[i].flowmeter.risingReady = true;
            }
        }
    }

    // if (!digitalRead(singleDos.flowmeter.getPin()))
    // {
    //     singleDos.flowmeter.risingLowCount++;
    //     if (singleDos.flowmeter.risingLowCount > 4)
    //     {
    //         singleDos.flowmeter.risingLowCount = 0;
    //         singleDos.flowmeter.risingReady = true;
    //     }
    // }
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
    for (int i = 0; i < nCollectors; i++)
    {
        collectors[i].loop();
    }
    // singleDos.loop();
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
    if (loopRunning_ && loopValveOk_ && loopPump_ && !loopCollector_)
    {
        loopCollectorStart();
    }

    // 3.2 start single dos
    // if (loopRunning_ && loopValveOk_ && loopPump_ && !loopSingleDos_ && singleDos.loopUsing_)
    // {
    //     singleDos.loopStart();
    //     loopSingleDos_ = true;
    //     Serial.println("Loop single dos Start!");
    // }

    // 4. need washing
    if (loopRunning_ && loopValveOk_ && loopPump_ && !loopDone_ && !loopWashing_)
    {
        if (abs(carrierDosedPercent) < carrierReserve)
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
        // if (collector.loopUsing_)
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
    // loopSingleDos_ = false;
    loopRunning_ = true;
    loopDone_ = false;

    loopWashing_ = false;
    loopCollectorWashingStop();

    // m1.nullifyVolume();
    // collector.loopStart();
}

void loopDevicesStop()
{
    for (int i = 0; i < nCollectors; i++)
    {
        collectors[i].loopStop();
    }
    // singleDos.loopStop();
}

void loopCollectorStart()
{
    for (int i = 0; i < nCollectors; i++)
    {
        if (collectors[i].loopUsing_)
        {
            collectors[i].loopStart();
            loopCollector_ = true;
            Serial.print("Loop collector #");
            Serial.print(i);
            Serial.println(" Start");
        }
    }
}

void loopCollectorWashingStart()
{
    for (int i = 0; i < nCollectors; i++)
    {
        if (collectors[i].loopUsing_)
        {
            collectors[i].loopStop();
            collectors[i].washingCarrierReserve_ = true;
            collectors[i].washingStart_ = true;
        }
    }
}

void loopCollectorWashingStop()
{
    for (int i = 0; i < nCollectors; i++)
    {
        if (collectors[i].loopUsing_)
        {
            collectors[i].washingCarrierReserve_ = false;
            collectors[i].resetWash();
            collectors[i].closeAll();
        }
    }
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
    // loopSingleDos_ = false;
    loopRunning_ = false;
    loopDone_ = false;

    loopWashing_ = false;
}

void loopPause()
{
    for (int i = 0; i < nCollectors; i++)
    {
        collectors[i].loopPause();
    }
}

void pumpStart()
{
    pumpCommand = true;
}

void pumpStop()
{
    pumpCommand = false;
}