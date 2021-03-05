#include "FallingEdge.h"

FallingEdge::FallingEdge()
{
}

bool FallingEdge::falling(bool enable)
{
    bool edge = !enable && prev_;
    prev_ = enable;
    return edge;
}