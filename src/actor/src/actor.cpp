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
        auto cameras = GetCharacteristicsByType("Camera");
        for (auto item : cameras)
        {
            auto camera = (Camera *)item;
            camera->RegisterRenderEvent();
        }

        auto renderers = GetCharacteristicsByType("Renderer");
        for (auto item : renderers)
        {
            auto renderer = (Renderer *)item;
            renderer->RegisterCameraDetectEvent();
        }

        auto behaviors = GetCharacteristicsByType("Behavior");
        for (auto item : behaviors)
        {
            auto behavior = (Behavior *)item;
            behavior->RegisterOnStart();
            behavior->RegisterOnUpdate();
            behavior->RegisterOnLateUpdate();
        }

        // recursively register events in sub actors
        for (auto sub_actor : _sub_actors)
        {
            sub_actor->RegisterSystemEvents();
        }
    }

    void Actor::UnregisterSystemEvents()
    {
        auto cameras = GetCharacteristicsByType("Camera");
        for (auto item : cameras)
        {
            auto camera = (Camera *)item;
            camera->UnregisterRenderEvent();
        }

        auto renderers = GetCharacteristicsByType("Renderer");
        for (auto item : renderers)
        {
            auto renderer = (Renderer *)item;
            renderer->UnregisterCameraDetectEvent();
        }

        auto behaviors = GetCharacteristicsByType("Behavior");
        for (auto item : behaviors)
        {
            auto behavior = (Behavior *)item;
            behavior->UnregisterOnStart();
            behavior->UnregisterOnUpdate();
            behavior->UnregisterOnLateUpdate();
        }

        // recursively unregister events in sub actors
        for (auto sub_actor : _sub_actors)
        {
            sub_actor->UnregisterSystemEvents();
        }
    }

    Actor *Actor::CreateSubActor(FPoint position, float rotate, FVect scale)
    {
        Actor *sub_actor = new Actor(this, position, rotate, scale);
        _sub_actors.push_back(sub_actor);
        return sub_actor;
    }
} // namespace gamo
