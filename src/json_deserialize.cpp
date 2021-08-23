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
}

void collectorLoopJSON(DynamicJsonDocument jsonReceived, Collector *collector)
{
    if (jsonReceived.containsKey("volumePulseCounter"))
    {
        collector->flowmeter.volumePulseCounter_ = jsonReceived["volumePulseCounter"];
        // Serial.println(collector->flowmeter.getVolume());
    }

    if (jsonReceived.containsKey("commandStart"))
        if (jsonReceived["commandStart"])
            loopCommand();

    if (jsonReceived.containsKey("commandStop"))
        if (jsonReceived["commandStop"])
            loopStop();

    // if (jsonReceived.containsKey("commandPause"))
    //     if (jsonReceived["commandPause"])
    //         loopPause();

    // if (jsonReceived.containsKey("doseCommandStart"))
    //     if (jsonReceived["doseCommandStart"])
    //         collector->doseCommand();

    // valveNums
    if (jsonReceived.containsKey("valveNums"))
    {
        for (int i = 0; i < jsonReceived["valveNums"].size(); i++)
        {
            collector->valveNums[i] = jsonReceived["valveNums"][i];
            Serial.println(collector->valveNums[i]);
        }
    }
    // requiredVolumes
    if (jsonReceived.containsKey("requiredVolumes"))
    {
        for (int i = 0; i < jsonReceived["requiredVolumes"].size(); i++)
        {
            collector->requiredVolumes[i] = jsonReceived["requiredVolumes"][i];
            Serial.println(collector->requiredVolumes[i]);
        }
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

void pumpJSON(DynamicJsonDocument jsonReceived)
{
    if (jsonReceived.containsKey("commandStart"))
        if (jsonReceived["commandStart"])
            pumpCommand = true;

    if (jsonReceived.containsKey("commandStop"))
        if (jsonReceived["commandStop"])
            pumpCommand = false;
}

void messageContainsKeys(DynamicJsonDocument jsonReceived)
{
    // if (jsonReceived.containsKey("loop"))
    //     loopJSON(jsonReceived["loop"]);

    if (jsonReceived.containsKey("showSettings"))
        showSettings = jsonReceived["showSettings"];

    if (jsonReceived.containsKey("ack"))
        ack = jsonReceived["ack"];

    if (jsonReceived.containsKey("pump"))
        pumpJSON(jsonReceived["pump"]);

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