#ifndef FALLING_EDGE_h
#define FALLING_EDGE_h

class FallingEdge
{
private:
    bool prev_;

public:
    FallingEdge();
    bool falling(bool enable);
};

#endif