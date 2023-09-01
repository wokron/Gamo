#pragma once

namespace gamo
{
    template<typename T>
    class IPrototype
    {
    public:
        virtual T *Clone() = 0;

        virtual ~IPrototype() {}; // declare the destructor as a virtual destructor
    };
    
} // namespace gamo
