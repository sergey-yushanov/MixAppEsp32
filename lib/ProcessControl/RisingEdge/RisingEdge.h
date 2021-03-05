#ifndef RISING_EDGE_h
#define RISING_EDGE_h

class RisingEdge
{
private:
    bool prev_;

public:
    RisingEdge();
    bool rising(bool enable);
};

#endif