#include "json_serialize.h"

DynamicJsonDocument valveJSON(int number, Valve valve)
{
    DynamicJsonDocument doc(1024);
    doc["number"] = number;
    doc["command"] = valve.isCommand();
    doc["faulty"] = valve.isFaulty();
    return doc;
}

DynamicJsonDocument valveAdjustableJSON(ValveAdjustable valveAdjustable, bool showSettings)
{
    DynamicJsonDocument doc(1024);
    doc["commandOpen"] = valveAdjustable.isCommandOpen();
    doc["commandClose"] = valveAdjustable.isCommandClose();
    doc["faulty"] = valveAdjustable.isFaulty();
    doc["setpoint"] = valveAdjustable.getSetpoint();
    doc["position"] = valveAdjustable.getPosition();
    if (showSettings)
    {
        doc["overtime"] = valveAdjustable.getOvertime();
        doc["limitClose"] = valveAdjustable.getLimitClose();
        doc["limitOpen"] = valveAdjustable.getLimitOpen();
        doc["deadbandClose"] = valveAdjustable.getDeadbandClose();
        doc["deadbandOpen"] = valveAdjustable.getDeadbandOpen();
        doc["deadbandPosition"] = valveAdjustable.getDeadbandPosition();
        doc["costClose"] = valveAdjustable.getCostClose();
        doc["costOpen"] = valveAdjustable.getCostOpen();
        doc["sensor"]["raw"] = valveAdjustable.getPositionSensor()->getRaw();
        doc["sensor"]["rawLowLimit"] = valveAdjustable.getPositionSensor()->getRawLowLimit();
        doc["sensor"]["rawHighLimit"] = valveAdjustable.getPositionSensor()->getRawHighLimit();
        doc["sensor"]["valueLowLimit"] = valveAdjustable.getPositionSensor()->getValueLowLimit();
        doc["sensor"]["valueHighLimit"] = valveAdjustable.getPositionSensor()->getValueHighLimit();
    }
    return doc;
}

DynamicJsonDocument flowmeterJSON(Flowmeter flowmeter, bool showSettings)
{
    DynamicJsonDocument doc(1024);
    doc["flow"] = flowmeter.getFlow();
    doc["volume"] = flowmeter.getVolume();
    if (showSettings)
    {
        doc["pulsesPerLiter"] = flowmeter.getPulsesPerLiter();
    }
    return doc;
}

DynamicJsonDocument dispenserCollectorJSON(int number, DispenserCollector dispenserCollector, bool showSettings)
{
    DynamicJsonDocument doc(1024);
    doc["number"] = number;
    doc["valveAdjustable"] = valveAdjustableJSON(dispenserCollector.valveAdjustable, showSettings);
    doc["flowmeter"] = flowmeterJSON(dispenserCollector.flowmeter, showSettings);
    for (int i = 0; i < dispenserCollector.nValves_; i++)
    {
        doc["valves"][i] = valveJSON(i + 1, dispenserCollector.valves[i]);
    }
    return doc;
}

DynamicJsonDocument commonJSON(ValveAdjustable valveAdjustable, Flowmeter flowmeter, bool showSettings)
{
    DynamicJsonDocument doc(1024);
    doc["valveAdjustable"] = valveAdjustableJSON(valveAdjustable, showSettings);
    doc["flowmeter"] = flowmeterJSON(flowmeter, showSettings);
    return doc;
}

String jsonSerialize(ValveAdjustable commonValveAdjustable, Flowmeter commonFlowmeter, DispenserCollector dispenserCollector, bool showSettings)
{
    // todo: создать класс, в котором содержатся все типы коллекторов с их количеством
    // todo: передать в функцию только один экземпляр этого класса
    String result;
    DynamicJsonDocument doc(2048);
    doc["common"] = commonJSON(commonValveAdjustable, commonFlowmeter, showSettings);
    doc["dispenserCollectors"][0] = dispenserCollectorJSON(1, dispenserCollector, showSettings);
    serializeJson(doc, result);
    return result;
}
