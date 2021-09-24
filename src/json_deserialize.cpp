#include "json_deserialize.h"

void flowmeterJSON(DynamicJsonDocument jsonReceived, Flowmeter *flowmeter)
{
    if (jsonReceived.containsKey("pulsesPerLiter"))
        flowmeter->setPulsesPerLiter(jsonReceived["pulsesPerLiter"]);
    if (jsonReceived.containsKey("nullifyVolume"))
        if (jsonReceived["nullifyVolume"])
            flowmeter->nullifyVolume();
}

void valveJSON(DynamicJsonDocument jsonReceived, Valve *valve)
{
    if (jsonReceived.containsKey("commandOpen"))
        if (jsonReceived["commandOpen"])
            valve->open();

    if (jsonReceived.containsKey("commandClose"))
        if (jsonReceived["commandClose"])
            valve->close();
}

void sensorJSON(DynamicJsonDocument jsonReceived, ValveAdjustable *valveAdjustable)
{
    if (jsonReceived.containsKey("rawLowLimit"))
        valveAdjustable->getPositionSensor()->setRawLowLimit(jsonReceived["rawLowLimit"]);

    if (jsonReceived.containsKey("rawHighLimit"))
        valveAdjustable->getPositionSensor()->setRawHighLimit(jsonReceived["rawHighLimit"]);

    if (jsonReceived.containsKey("valueLowLimit"))
        valveAdjustable->getPositionSensor()->setValueLowLimit(jsonReceived["valueLowLimit"]);

    if (jsonReceived.containsKey("valueHighLimit"))
        valveAdjustable->getPositionSensor()->setValueHighLimit(jsonReceived["valueHighLimit"]);
}

void valveAdjustableJSON(DynamicJsonDocument jsonReceived, ValveAdjustable *valveAdjustable)
{
    if (jsonReceived.containsKey("commandOpen"))
        if (jsonReceived["commandOpen"])
        {
            valveAdjustable->fullyOpen();
            openTimeBegin = millis();
            openTimeCalc = true;
        }

    if (jsonReceived.containsKey("commandClose"))
        if (jsonReceived["commandClose"])
        {
            valveAdjustable->fullyClose();
            closeTimeBegin = millis();
            closeTimeCalc = true;
        }

    if (jsonReceived.containsKey("setpoint"))
        valveAdjustable->setSetpoint(jsonReceived["setpoint"]);

    if (jsonReceived.containsKey("overtime"))
        valveAdjustable->setOvertime(jsonReceived["overtime"]);

    if (jsonReceived.containsKey("limitClose"))
        valveAdjustable->setLimitClose(jsonReceived["limitClose"]);

    if (jsonReceived.containsKey("limitOpen"))
        valveAdjustable->setLimitOpen(jsonReceived["limitOpen"]);

    if (jsonReceived.containsKey("deadbandClose"))
        valveAdjustable->setDeadbandClose(jsonReceived["deadbandClose"]);

    if (jsonReceived.containsKey("deadbandOpen"))
        valveAdjustable->setDeadbandOpen(jsonReceived["deadbandOpen"]);

    if (jsonReceived.containsKey("deadbandPosition"))
        valveAdjustable->setDeadbandPosition(jsonReceived["deadbandPosition"]);

    if (jsonReceived.containsKey("costClose"))
        valveAdjustable->setCostClose(jsonReceived["costClose"]);

    if (jsonReceived.containsKey("costOpen"))
        valveAdjustable->setCostOpen(jsonReceived["costOpen"]);

    if (jsonReceived.containsKey("sensor"))
        sensorJSON(jsonReceived["sensor"], valveAdjustable);
}

void commonLoopJSON(DynamicJsonDocument jsonReceived)
{
    // carrierRequiredVolume
    if (jsonReceived.containsKey("carrierRequiredVolume"))
    {
        carrierRequiredVolume = jsonReceived["carrierRequiredVolume"];
        carrierDosedVolume = 0;
        // Serial.println(carrierRequiredVolume);
    }

    if (jsonReceived.containsKey("commandStart"))
        if (jsonReceived["commandStart"])
            loopStart();

    if (jsonReceived.containsKey("commandStop"))
        if (jsonReceived["commandStop"])
            loopStop();

    if (jsonReceived.containsKey("commandPause"))
        if (jsonReceived["commandPause"])
            loopPause();

    if (jsonReceived.containsKey("valveSetpoint"))
        valveSetpoint = jsonReceived["valveSetpoint"];

    if (jsonReceived.containsKey("carrierReserve"))
        carrierReserve = jsonReceived["carrierReserve"];
}

void commonJSON(DynamicJsonDocument jsonReceived)
{
    if (jsonReceived.containsKey("valveAdjustable"))
    {
        valveAdjustableJSON(jsonReceived["valveAdjustable"], &valveAdjustable);
        // dataSave(preferences_common, &valveAdjustable);
    }

    if (jsonReceived.containsKey("flowmeter"))
    {
        flowmeterJSON(jsonReceived["flowmeter"], &m1);
        // dataSave(preferences_common, &m1);
    }

    if (jsonReceived.containsKey("loop"))
    {
        commonLoopJSON(jsonReceived["loop"]);
    }
}

void collectorLoopJSON(DynamicJsonDocument jsonReceived, Collector *collector)
{
    collector->loopStop();

    if (jsonReceived.containsKey("volumePulseCounter"))
    {
        collector->flowmeter.volumePulseCounter_ = jsonReceived["volumePulseCounter"];
        // Serial.println(collector->flowmeter.getVolume());
    }

    if (jsonReceived.containsKey("ratioVolume0"))
    {
        collector->setRatioVolume0(jsonReceived["ratioVolume0"]);
    }

    if (jsonReceived.containsKey("ratioVolume1"))
    {
        collector->setRatioVolume1(jsonReceived["ratioVolume1"]);
    }

    if (jsonReceived.containsKey("ratioVolume2"))
    {
        collector->setRatioVolume2(jsonReceived["ratioVolume2"]);
    }

    // if (jsonReceived.containsKey("ratioVolume"))
    // {
    //     collector->setRatioVolume(jsonReceived["ratioVolume"]);
    //     // ratioVolume = jsonReceived["ratioVolume"];
    // }

    // if (jsonReceived.containsKey("ratioVolumeMicro"))
    // {
    //     collector->setRatioVolumeMicro(jsonReceived["ratioVolumeMicro"]);
    //     // ratioVolumeMicro = jsonReceived["ratioVolumeMicro"];
    // }

    if (jsonReceived.containsKey("volume1"))
    {
        collector->setVolume1(jsonReceived["volume1"]);
    }

    if (jsonReceived.containsKey("volume2"))
    {
        collector->setVolume2(jsonReceived["volume2"]);
    }

    // if (jsonReceived.containsKey("microVolume"))
    // {
    //     collector->setMicroVolume(jsonReceived["microVolume"]);
    // }

    if (jsonReceived.containsKey("setpoint1"))
    {
        collector->setSetpoint1(jsonReceived["setpoint1"]);
    }

    if (jsonReceived.containsKey("setpoint2"))
    {
        collector->setSetpoint2(jsonReceived["setpoint2"]);
    }

    // if (jsonReceived.containsKey("microSetpoint"))
    // {
    //     collector->setMicroSetpoint(jsonReceived["microSetpoint"]);
    // }

    // if (jsonReceived.containsKey("commandStart"))
    //     if (jsonReceived["commandStart"])
    //         loopStart();

    // if (jsonReceived.containsKey("commandStop"))
    //     if (jsonReceived["commandStop"])
    //         loopStop();

    // if (jsonReceived.containsKey("commandPause"))
    //     if (jsonReceived["commandPause"])
    //         loopPause();

    // if (jsonReceived.containsKey("doseCommandStart"))
    //     if (jsonReceived["doseCommandStart"])
    //         collector->doseCommand();

    // valveNums
    if (jsonReceived.containsKey("valveNums"))
    {
        // for (int i = 0; i < collector->nValves_; i++)
        // {
        //     collector->valveNums[i] = 0;
        // }

        for (int i = 0; i < collector->nValves_ - 1; i++)
        {
            collector->valveNums[i] = 0;
        }

        for (int i = 0; i < jsonReceived["valveNums"].size(); i++)
        {
            collector->valveNums[i] = jsonReceived["valveNums"][i];
            // Serial.println(collector->valveNums[i]);
        }
    }
    // requiredVolumes
    collector->loopUsing_ = false;
    if (jsonReceived.containsKey("requiredVolumes"))
    {
        for (int i = 0; i < collector->nValves_ - 1; i++)
        {
            collector->requiredVolumes[i] = 0;
            collector->dosedVolumes[i] = 0;
        }

        for (int i = 0; i < jsonReceived["requiredVolumes"].size(); i++)
        {
            collector->requiredVolumes[i] = jsonReceived["requiredVolumes"][i];

            if (collector->requiredVolumes[i] > 0)
                collector->loopUsing_ = true;
            // Serial.println(collector->requiredVolumes[i]);
        }
    }
}

void singleDosLoopJSON(DynamicJsonDocument jsonReceived, SingleDos *singleDos)
{
    singleDos->loopStop();

    if (jsonReceived.containsKey("volumePulseCounter"))
    {
        singleDos->flowmeter.volumePulseCounter_ = jsonReceived["volumePulseCounter"];
        // Serial.println(collector->flowmeter.getVolume());
    }

    if (jsonReceived.containsKey("ratioVolume"))
    {
        singleDos->setRatioVolume(jsonReceived["ratioVolume"]);
        // ratioVolumeMicro = jsonReceived["ratioVolume"];
    }

    if (jsonReceived.containsKey("ratioVolumeMicro"))
    {
        singleDos->setRatioVolumeMicro(jsonReceived["ratioVolumeMicro"]);
        // ratioVolumeMicro = jsonReceived["ratioVolumeMicro"];
    }

    // requiredVolume
    singleDos->loopUsing_ = false;
    if (jsonReceived.containsKey("requiredVolume"))
    {
        singleDos->requiredVolume = jsonReceived["requiredVolume"];
        singleDos->dosedVolume = 0;

        if (singleDos->requiredVolume > 0)
            singleDos->loopUsing_ = true;
        // Serial.println(singleDos->requiredVolume);
    }
}

void collectorJSON(DynamicJsonDocument jsonReceived, Collector *collector)
{
    if (jsonReceived.containsKey("valveAdjustable"))
    {
        valveAdjustableJSON(jsonReceived["valveAdjustable"], &collector->valveAdjustable);
        // dataSave(preferences_dispenser_0, &collector->valveAdjustable);
    }

    if (jsonReceived.containsKey("flowmeter"))
    {
        flowmeterJSON(jsonReceived["flowmeter"], &collector->flowmeter);
        // dataSave(preferences_dispenser_0, &collector->flowmeter);
    }

    if (jsonReceived.containsKey("valves"))
    {
        for (int i = 0; i < jsonReceived["valves"].size(); i++)
        {
            if (jsonReceived["valves"][i].containsKey("number"))
            {
                int valveNum = jsonReceived["valves"][i]["number"];
                valveNum--;
                valveJSON(jsonReceived["valves"][i], &collector->valves[valveNum]);
            }
        }
    }

    if (jsonReceived.containsKey("loop"))
    {
        collectorLoopJSON(jsonReceived["loop"], collector);
    }
}

void singleDosJSON(DynamicJsonDocument jsonReceived, SingleDos *singleDos)
{
    if (jsonReceived.containsKey("valveAdjustable"))
    {
        valveAdjustableJSON(jsonReceived["valveAdjustable"], &singleDos->valveAdjustable);
        // dataSave(preferences_dispenser_0, &collector->valveAdjustable);
    }

    if (jsonReceived.containsKey("flowmeter"))
    {
        flowmeterJSON(jsonReceived["flowmeter"], &singleDos->flowmeter);
        // dataSave(preferences_dispenser_0, &collector->flowmeter);
    }

    if (jsonReceived.containsKey("loop"))
    {
        singleDosLoopJSON(jsonReceived["loop"], singleDos);
    }
}

void pumpJSON(DynamicJsonDocument jsonReceived)
{
    // if (jsonReceived.containsKey("pumpStart"))
    //     if (jsonReceived["pumpStart"])
    //         pumpStart();

    // if (jsonReceived.containsKey("pumpStop"))
    //     if (jsonReceived["pumpStop"])
    //         pumpStop();
}

void messageContainsKeys(DynamicJsonDocument jsonReceived)
{
    // if (jsonReceived.containsKey("loop"))
    //     loopJSON(jsonReceived["loop"]);

    if (jsonReceived.containsKey("showSettings"))
        showSettings = jsonReceived["showSettings"];

    if (jsonReceived.containsKey("ack"))
        ack = jsonReceived["ack"];

    if (jsonReceived.containsKey("pumpStart"))
        if (jsonReceived["pumpStart"])
            pumpStart();

    if (jsonReceived.containsKey("pumpStop"))
        if (jsonReceived["pumpStop"])
            pumpStop();

    // if (jsonReceived.containsKey("pump"))
    //     pumpJSON(jsonReceived["pump"]);

    if (jsonReceived.containsKey("common"))
        commonJSON(jsonReceived["common"]);

    if (jsonReceived.containsKey("collectors"))
    {
        for (int i = 0; i < jsonReceived["collectors"].size(); i++)
        {
            if (jsonReceived["collectors"][i].containsKey("number"))
            {
                int collectorNum = jsonReceived["collectors"][i]["number"];
                collectorNum--;
                if (collectorNum == 0)
                    collectorJSON(jsonReceived["collectors"][i], &collector);
            }
        }
    }

    if (jsonReceived.containsKey("singles"))
    {
        for (int i = 0; i < jsonReceived["singles"].size(); i++)
        {
            if (jsonReceived["singles"][i].containsKey("number"))
            {
                int singleDosNum = jsonReceived["singles"][i]["number"];
                singleDosNum--;
                if (singleDosNum == 0)
                    singleDosJSON(jsonReceived["singles"][i], &singleDos);
            }
        }
    }
}

void jsonDeserialize(uint8_t *data)
{
    // Allocate the received JSON
    DynamicJsonDocument doc(1024);

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, data);
    // Test if parsing succeeds.
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    // check message keys
    messageContainsKeys(doc);
}