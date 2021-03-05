#include "RisingEdge.h"

RisingEdge::RisingEdge()
{
}

bool RisingEdge::rising(bool enable)
{
    bool edge = enable && !prev_;
    prev_ = enable;
    return edge;
}