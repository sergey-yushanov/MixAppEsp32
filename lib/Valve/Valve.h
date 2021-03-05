#ifndef VALVE_h
#define VALVE_h

#include <Arduino.h>

class Valve
{
private:
    int overtime_;
    int time_;

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
    bool healthy_;
    bool requestOpen_;
    bool requestClose_;
    bool positionOpen_;
    bool positionClose_;
    bool positionOk_;
    bool commandOpen_;
    bool commandClose_;
    bool command_;

    bool enable_;
    bool disable_;
    bool fullyOpen_;
    bool fullyClose_;
    bool manual_;
    bool auto_;

    void calcTimeout(bool _0_1s);
    void checkFaulty(bool ack);
    void checkPosition();
    void checkRequest();
    void checkCommand();
    void checkStatus();
    void request();
    void checkMode();
    void resetRequest();

public:
    int status;

    Valve();
    Valve(int overtime, float limitClose, float limitOpen, float deadbandClose, float deadbandOpen, float deadbandPosition, float costClose, float costOpen);
    void process(bool _0_1s, bool ack);
    void process(float setpoint, float position, bool _0_1s, bool ack);
    void setSetpoint(float setpoint);
    void setPosition(float position);
    void setOvertime(int overtime);
    void setLimits(float limitClose, float limitOpen);
    void setDeadbands(float deadbandClose, float deadbandOpen, float deadbandPosition);
    void setCosts(float costClose, float costOpen);
};

#endif