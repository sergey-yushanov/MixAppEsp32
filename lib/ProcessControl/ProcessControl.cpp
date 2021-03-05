//#include <PCA.h>

//extern long currentMillis;

// void OnTimer::onTimer(bool _en, int _period, TS _step)
// {
//     if (!_en)
//     {
//         startMillis = millis();
//         periodMillis = _period * _step;
//     }
//     status = _en && (millis() - startMillis >= periodMillis);
// }

// void OffTimer::offTimer(bool _en, int _period, TS _step)
// {
//     if (_en)
//     {
//         startMillis = millis();
//         periodMillis = _period * _step;
//     }
//     status = _en || (millis() - startMillis <= periodMillis);
// }

// bool PosEdge::posEdge(bool _en)
// {
//     bool edge = _en && !prev;
//     prev = _en;
//     return edge;
// }

// bool NegEdge::negEdge(bool _en)
// {
//     bool edge = !_en && prev;
//     prev = _en;
//     return edge;
// }