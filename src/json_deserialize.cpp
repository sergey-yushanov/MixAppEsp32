#include "json_deserialize.h"

void flowmeterJSON(DynamicJsonDocument jsonReceived, Flowmeter *flowmeter)
{
    if (jsonReceived.containsKey("pulsesPerLiter"))
        flowmeter->setPulsesPerLiter(jsonReceived["pulsesPerLiter"]);
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
            valveAdjustable->fullyOpen();

    if (jsonReceived.containsKey("commandClose"))
        if (jsonReceived["commandClose"])
            valveAdjustable->fullyClose();

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

void dispenserCollectorJSON(DynamicJsonDocument jsonReceived, DispenserCollector *dispenserCollector)
{
    if (jsonReceived.containsKey("valveAdjustable"))
    {
        valveAdjustableJSON(jsonReceived["valveAdjustable"], &dispenserCollector->valveAdjustable);
        // dataSave(preferences_dispenser_0, &dispenserCollector->valveAdjustable);
    }

    if (jsonReceived.containsKey("flowmeter"))
    {
        flowmeterJSON(jsonReceived["flowmeter"], &dispenserCollector->flowmeter);
        // dataSave(preferences_dispenser_0, &dispenserCollector->flowmeter);
    }

    if (jsonReceived.containsKey("valves"))
    {
        for (int i = 0; i < jsonReceived["valves"].size(); i++)
        {
            if (jsonReceived["valves"][i].containsKey("number"))
            {
                int valveNum = jsonReceived["valves"][i]["number"];
                valveNum--;
                valveJSON(jsonReceived["valves"][i], &dispenserCollector->valves[valveNum]);
            }
        }
    }
}

void messageContainsKeys(DynamicJsonDocument jsonReceived)
{
    if (jsonReceived.containsKey("showSettings"))
        showSettings = jsonReceived["showSettings"];

    if (jsonReceived.containsKey("ack"))
        ack = jsonReceived["ack"];

    if (jsonReceived.containsKey("common"))
        commonJSON(jsonReceived["common"]);

    if (jsonReceived.containsKey("dispenserCollectors"))
    {
        for (int i = 0; i < jsonReceived["dispenserCollectors"].size(); i++)
        {
            if (jsonReceived["dispenserCollectors"][i].containsKey("number"))
            {
                int dispenserCollectorNum = jsonReceived["dispenserCollectors"][i]["number"];
                dispenserCollectorNum--;
                if (dispenserCollectorNum == 0)
                    dispenserCollectorJSON(jsonReceived["dispenserCollectors"][i], &dispenserCollector);
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