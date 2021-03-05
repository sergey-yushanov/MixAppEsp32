/*
    Process control automation blocks
*/

#ifndef PROCESS_CONTROL_h
#define PROCESS_CONTROL_h

#include <OnTimer/OnTimer.h>
#include <OffTimer/OffTimer.h>
#include <RisingEdge/RisingEdge.h>
#include <FallingEdge/FallingEdge.h>

// timer step
// enum TimerStep : long
// {
//     _10ms = 10,
//     _100ms = 100,
//     _1s = 1000
// };

// delay on timer
// class OnTimer
// {
// private:
//     long periodMillis;
//     long startMillis;
//     bool status;

// public:
//     OnTimer();
//     void onTimer(bool en, int period, TimerStep timerStep);
// };

// // delay off timer
// struct OffTimer
// {
//     long periodMillis;
//     long startMillis;

//     bool en;
//     bool enFalling;
//     bool enPrev;
//     bool switchedOn;
//     bool status;

//     void offTimer(bool en, int period, TimerStep timerStep);
// };

// // positive edge
// struct PosEdge
// {
//     bool prev;
//     bool posEdge(bool _en);
// };

// // negative edge
// struct NegEdge
// {
//     bool prev;
//     bool negEdge(bool _en);
// };

// struct Timer
// {
//     long periodMillis;
//     long startMillis;
//     long currentMilllis;

//     bool enRising;
//     bool enFalling;
//     bool enPrev;
//     bool status;

//     void process(long _periodMillis, bool _en, bool _0_1s);
//     void edgeDetect(bool _en);
//     bool onTimer(bool en);
//     bool offTimer(bool en);
// };

#endif
