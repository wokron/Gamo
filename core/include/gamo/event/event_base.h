#pragma once

#include "eventtype.h"

namespace gamo
{
    class Event
    {
    public:
        virtual int Type() const { return EVENT_BASE; }
    };
} // namespace gamo
