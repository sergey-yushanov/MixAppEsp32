#include "save_data.h"

Preferences preferences;

const char *preferences_common = "common";
const char *preferences_collector_0 = "collector_0";

void valveAdjustableRead(ValveAdjustable *valveAdjustable)
{
    int overtime = preferences.getInt("overtime", 150);
    valveAdjustable->setOvertime(overtime);
    float limitClose = preferences.getFloat("limitClose", 0);
    valveAdjustable->setLimitClose(limitClose);
    float limitOpen = preferences.getFloat("limitOpen", 100);
    valveAdjustable->setLimitOpen(limitOpen);
    float deadbandClose = preferences.getFloat("deadbandClose", 0);
    valveAdjustable->setDeadbandClose(deadbandClose);
    float deadbandOpen = preferences.getFloat("deadbandOpen", 1);
    valveAdjustable->setDeadbandOpen(deadbandOpen);
    float deadbandPosition = preferences.getFloat("deadbandPosition", 1);
    valveAdjustable->setDeadbandPosition(deadbandPosition);
    float costClose = preferences.getFloat("costClose", 1);
    valveAdjustable->setCostClose(costClose);
    float costOpen = preferences.getFloat("costOpen", 1);
    valveAdjustable->setCostOpen(costOpen);

    float rawLowLimit = preferences.getFloat("rawLowLimit", 0);
    valveAdjustable->getPositionSensor()->setRawLowLimit(rawLowLimit);
    float rawHighLimit = preferences.getFloat("rawHighLimit", 65536);
    valveAdjustable->getPositionSensor()->setRawHighLimit(rawHighLimit);
    float valueLowLimit = preferences.getFloat("valueLowLimit", 0);
    valveAdjustable->getPositionSensor()->setValueLowLimit(valueLowLimit);
    float valueHighLimit = preferences.getFloat("valueHighLimit", 100);
    valveAdjustable->getPositionSensor()->setValueHighLimit(valueHighLimit);
}

void flowmeterRead(Flowmeter *flowmeter, float defaultPulsesPerLiter)
{
    float pulsesPerLiter = preferences.getFloat("pulsesPerLiter", defaultPulsesPerLiter);
    flowmeter->setPulsesPerLiter(pulsesPerLiter);
}

void valveAdjustableWrite(ValveAdjustable *valveAdjustable)
{
    preferences.putInt("overtime", valveAdjustable->getOvertime());
    preferences.putFloat("limitClose", valveAdjustable->getLimitClose());
    preferences.putFloat("limitOpen", valveAdjustable->getLimitOpen());
    preferences.putFloat("deadbandClose", valveAdjustable->getDeadbandClose());
    preferences.putFloat("deadbandOpen", valveAdjustable->getDeadbandOpen());
    preferences.putFloat("deadbandPosition", valveAdjustable->getDeadbandPosition());
    preferences.putFloat("costClose", valveAdjustable->getCostClose());
    preferences.putFloat("costOpen", valveAdjustable->getCostOpen());

    preferences.putFloat("rawLowLimit", valveAdjustable->getPositionSensor()->getRawLowLimit());
    preferences.putFloat("rawHighLimit", valveAdjustable->getPositionSensor()->getRawHighLimit());
    preferences.putFloat("valueLowLimit", valveAdjustable->getPositionSensor()->getValueLowLimit());
    preferences.putFloat("valueHighLimit", valveAdjustable->getPositionSensor()->getValueHighLimit());
}

void flowmeterWrite(Flowmeter *flowmeter)
{
    preferences.getFloat("pulsesPerLiter", flowmeter->getPulsesPerLiter());
}

void dataSave(const char *key, ValveAdjustable *valveAdjustable)
{
    preferences.begin(key, false);
    valveAdjustableWrite(valveAdjustable);
    preferences.end();
}

void dataSave(const char *key, Flowmeter *flowmeter)
{
    preferences.begin(key, false);
    flowmeterWrite(flowmeter);
    preferences.end();
}

void dataSetup()
{
    // common settings
    preferences.begin(preferences_common, false);
    valveAdjustableRead(&valveAdjustable);
    flowmeterRead(&m1, 50.0);
    preferences.end();

    // dispenser collector settings
    preferences.begin(preferences_collector_0, false);
    valveAdjustableRead(&collector.valveAdjustable);
    flowmeterRead(&collector.flowmeter, 106.777);
    preferences.end();
}
