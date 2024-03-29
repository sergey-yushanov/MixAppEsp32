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
        // int valveNum = collector.valveNums[i] - 1;
        doc["valveNums"][i] = collector.valveNums[i];
        doc["requiredVolumes"][i] = collector.requiredVolumes[i];
        doc["dosedVolumes"][i] = collector.dosedVolumes[i];
        // doc["dosedVolumes"][i] = random(1000);
    }

    doc["ratioVolume0"] = collector.ratioVolume0_;
    doc["ratioVolume1"] = collector.ratioVolume1_;
    doc["ratioVolume2"] = collector.ratioVolume2_;

    doc["volume1"] = collector.volume1_;
    doc["volume2"] = collector.volume2_;

    doc["setpoint1"] = collector.setpoint1_;
    doc["setpoint2"] = collector.setpoint2_;

    // doc["ratioVolume"] = collector.ratioVolume_;
    // doc["microVolume"] = collector.microVolume_;
    // doc["microSetpoint"] = collector.microSetpoint_;
    // doc["ratioVolumeMicro"] = collector.ratioVolumeMicro_;
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
    // doc["carrierDosedVolume"] = random(1000);
    doc["carrierReserve"] = carrierReserve;
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

DynamicJsonDocument singleDosLoopJSON(SingleDos singleDos)
{
    DynamicJsonDocument doc(1024);
    doc["requiredVolume"] = singleDos.requiredVolume;
    doc["dosedVolume"] = singleDos.dosedVolume;
    return doc;
}

DynamicJsonDocument singleDosJSON(int number, SingleDos singleDos, bool showSettings)
{
    DynamicJsonDocument doc(1024);
    doc["number"] = number;
    doc["valveAdjustable"] = valveAdjustableJSON(singleDos.valveAdjustable, showSettings);
    doc["flowmeter"] = flowmeterJSON(singleDos.flowmeter, showSettings);
    doc["loop"] = singleDosLoopJSON(singleDos);
    return doc;
}

String jsonSerialize(ValveAdjustable commonValveAdjustable, Flowmeter commonFlowmeter, Collector collector, SingleDos singleDos, bool showSettings)
{
    // todo: создать класс, в котором содержатся все типы коллекторов с их количеством
    // todo: передать в функцию только один экземпляр этого класса
    String result;
    DynamicJsonDocument doc(2048);
    doc["showSettings"] = showSettings;
    doc["common"] = commonJSON(commonValveAdjustable, commonFlowmeter, showSettings);
    doc["collectors"][0] = collectorJSON(1, collector, showSettings);
    doc["singles"][0] = singleDosJSON(1, singleDos, showSettings);
    serializeJson(doc, result);
    return result;
}
