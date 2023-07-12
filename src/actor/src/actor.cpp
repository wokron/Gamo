#include "actor.h"
#include <algorithm>

namespace gamo
{
    Actor::Actor(FPoint position, float rotate, FVect scale)
    {
        _transform = new Transform(this, position, rotate, scale);
        _characteristics.push_back(_transform);
    }

    Actor::~Actor()
    {
        for (auto sub_actor : _characteristics)
        {
            delete sub_actor;
        }
    }

    Characteristic *Actor::GetCharacteristicByType(std::string type)
    {
        auto found = std::find_if(
            _characteristics.begin(), _characteristics.end(),
            [type](Characteristic *ch)
            { return ch->Type() == type; });
        if (found == _characteristics.end())
        {
            return nullptr;
        }
        else
        {
            return *found;
        }
    }

    Actor *Actor::CreateSubActor(FPoint position, float rotate, FVect scale)
    {
        Actor *sub_actor = new Actor(this, position, rotate, scale);
        _sub_actors.push_back(sub_actor);
        return sub_actor;
    }
} // namespace gamo
