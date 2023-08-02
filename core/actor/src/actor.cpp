#include "actor.h"
#include "scene.h"
#include <algorithm>

namespace gamo
{
    Actor::Actor(Vect position, float rotate, Vect scale)
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

    std::vector<Characteristic *> Actor::GetCharacteristicsByType(std::string type)
    {
        std::vector<Characteristic *> result;

        auto cmp = [type](Characteristic *ch)
        { return ch->Type() == type; };

        auto it = _characteristics.begin();
        while ((it = std::find_if(it, _characteristics.end(), cmp)) != _characteristics.end())
        {
            result.push_back(*it);
            it++;
        }

        return result;
    }

    void Actor::RegisterSystemEvents()
    {
        for (auto item : GetCharacteristics())
        {
            item->RegisterEvents();
        }

        // recursively register events in sub actors
        for (auto sub_actor : _sub_actors)
        {
            sub_actor->RegisterSystemEvents();
        }
    }

    void Actor::UnregisterSystemEvents()
    {
        for (auto item : GetCharacteristics())
        {
            item->UnregisterEvents();
        }

        // recursively unregister events in sub actors
        for (auto sub_actor : _sub_actors)
        {
            sub_actor->UnregisterSystemEvents();
        }
    }

    Actor *Actor::CreateSubActor(Vect position, float rotate, Vect scale)
    {
        Actor *sub_actor = new Actor(this, position, rotate, scale);
        _sub_actors.push_back(sub_actor);
        return sub_actor;
    }

    void Actor::Destroy()
    {
        if (_belong_scene != nullptr)
        {
            _belong_scene->RemoveResource(this);
        }
    }

    void Actor::Deref()
    {
        RegisterHandleMemFree();
    }

} // namespace gamo
