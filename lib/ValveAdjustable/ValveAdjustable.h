#ifndef VALVE_ADJUSTABLE_h
#define VALVE_ADJUSTABLE_h

#include <Arduino.h>
#include <AnalogSensor.h>

class ValveAdjustable
{
private:
    int overtime_;
    int time_;
    int status_;

    float position_;
    float setpoint_;
    float setpointManual_;
    float setpointAuto_;
    float error_;
    float errorAbs_;

    float limitOpen_;
    float limitClose_;
    float deadbandOpen_;
    float deadbandClose_;
    float deadbandPosition_;
    float costOpen_;
    float costClose_;

    bool faulty_;
    bool timeout_;

    bool requestOpen_;
    bool requestClose_;
    bool positionOpen_;
    bool positionClose_;
    bool positionOk_;
    bool command_;
    bool commandOpen_;
    bool commandClose_;

    AnalogSensor positionSensor_;

    void setPosition(float position);

    void setFaulty();
    void setRequest();
    void setCommand();

public:
    ValveAdjustable();

    void incTimeout();
    int getTime();

    bool isFaulty();
    void resetFaulty();
    int getStatus();

    bool isCommandOpen();
    bool isCommandClose();

    AnalogSensor *getPositionSensor();

    float getSetpoint();
    void setSetpoint(float setpoint);

    float getPosition();
    void updatePosition();

    int getOvertime();
    void setOvertime(int overtime = 300);

    float getLimitClose();
    float getLimitOpen();
    void setLimitClose(float limitClose = 0.0);
    void setLimitOpen(float limitOpen = 100.0);

    float getDeadbandClose();
    float getDeadbandOpen();
    float getDeadbandPosition();
    void setDeadbandClose(float deadbandClose = 1.0);
    void setDeadbandOpen(float deadbandOpen = 1.0);
    void setDeadbandPosition(float deadbandPosition = 1.0);

    float getCostClose();
    float getCostOpen();
    void setCostClose(float costClose = 1.0);
    void setCostOpen(float costOpen = 1.0);

    void fullyOpen();
    void fullyClose();
};

#endif