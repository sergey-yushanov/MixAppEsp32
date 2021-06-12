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

    void setOvertime(int overtime);
    void setLimits(float limitClose, float limitOpen);
    void setDeadbands(float deadbandClose, float deadbandOpen, float deadbandPosition);
    void setCosts(float costClose, float costOpen);

    void fullyOpen();
    void fullyClose();
};

#endif