#pragma once

#include <vector>
#include <string>

namespace gamo
{
    class Actor;
    class Transform;

    class Characteristic
    {
    private:
        Actor *_belong_actor;

    public:
        Characteristic(Actor *actor) { _belong_actor = actor; }

        Actor *BelongActor() { return _belong_actor; }

        std::vector<Characteristic *> &GetCharacteristics();

        Transform *GetTransform();

        /// @brief get the type of the characteristic,
        /// this is used to solve the problem that there is no method like "instanceof" in c++
        /// @return the type of characteristic
        virtual std::string Type() = 0;
    };
}