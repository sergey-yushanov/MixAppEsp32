#include "json_serialize.h"

float round2Dec(float value, int decimals)
{
    float multFactor = pow(10, decimals);
    return ((float)((int)(value * multFactor)) / multFactor);
}

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
    doc["position"] = round2Dec(valveAdjustable.getPosition(), 1);
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
    doc["flow"] = round2Dec(flowmeter.getFlow(), 2);
    doc["volume"] = round2Dec(flowmeter.getVolume(), 2);
    if (showSettings)
    {
        doc["pulsesPerLiter"] = flowmeter.getPulsesPerLiter();
    }
    return doc;
}

DynamicJsonDocument collectorLoopJSON(Collector collector)
{
    DynamicJsonDocument doc(1024);
    for (int i = 0; i < collector.nValves_ - 1; i++)
    {
        doc["valveNums"][i] = collector.valveNums[i];
        doc["requiredVolumes"][i] = collector.requiredVolumes[i];
        doc["dosedVolumes"][i] = collector.dosedVolumes[i];
    }
    return doc;
}

DynamicJsonDocument collectorJSON(int number, Collector collector, bool showSettings)
{
    DynamicJsonDocument doc(1024);
    doc["number"] = number;
    doc["valveAdjustable"] = valveAdjustableJSON(collector.valveAdjustable, showSettings);
    doc["flowmeter"] = flowmeterJSON(collector.flowmeter, showSettings);
    for (int i = 0; i < collector.nValves_; i++)
    {
        doc["valves"][i] = valveJSON(i + 1, collector.valves[i]);
    }
    doc["loop"] = collectorLoopJSON(collector);
    return doc;
}

DynamicJsonDocument commonLoopJSON()
{
    DynamicJsonDocument doc(1024);
    doc["carrierRequiredVolume"] = carrierRequiredVolume;
    doc["carrierDosedVolume"] = carrierDosedVolume;
    return doc;
}

DynamicJsonDocument commonJSON(ValveAdjustable valveAdjustable, Flowmeter flowmeter, bool showSettings)
{
    DynamicJsonDocument doc(1024);
    doc["valveAdjustable"] = valveAdjustableJSON(valveAdjustable, showSettings);
    doc["flowmeter"] = flowmeterJSON(flowmeter, showSettings);
    doc["loop"] = commonLoopJSON();
    return doc;
}

String jsonSerialize(ValveAdjustable commonValveAdjustable, Flowmeter commonFlowmeter, Collector collector, bool showSettings)
{
    // todo: создать класс, в котором содержатся все типы коллекторов с их количеством
    // todo: передать в функцию только один экземпляр этого класса
    String result;
    DynamicJsonDocument doc(2048);
    doc["showSettings"] = showSettings;
    doc["common"] = commonJSON(commonValveAdjustable, commonFlowmeter, showSettings);
    doc["collectors"][0] = collectorJSON(1, collector, showSettings);
    serializeJson(doc, result);
    return result;
}
