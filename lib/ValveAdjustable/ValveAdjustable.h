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

    float getSetpoint();
    void setSetpoint(float setpoint);

    float getPosition();
    void setPosition(float position);

    void setOvertime(int overtime = 150);
    void setLimits(float limitClose = 0.0, float limitOpen = 100.0);
    void setDeadbands(float deadbandClose = 1.0, float deadbandOpen = 1.0, float deadbandPosition = 1.0);
    void setCosts(float costClose = 1.0, float costOpen = 1.0);

    void fullyOpen();
    void fullyClose();
};

#endif