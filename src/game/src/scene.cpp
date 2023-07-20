#include "scene.h"
#include "director.h"
#include "actor.h"

namespace gamo
{
    void Scene::AddActor(Actor *actor)
    {
        _actors.push_back(actor);
    }

    int Scene::RenderStep()
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_RENDER, nullptr);
        return RenderDirector::GetInstance()->Render();
    }

    void Scene::PhysicsStep()
    {
        // todo: need to implement
    }

    void Scene::LogicStep()
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_UPDATE, nullptr);
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_LATE_UPDATE, nullptr);
    }

    void Scene::RegisterSystemEvents()
    {
        for (auto actor : _actors)
        {
            actor->RegisterSystemEvents();
        }
    }

    void Scene::UnregisterSystemEvents()
    {
        for (auto actor : _actors)
        {
            actor->UnregisterSystemEvents();
        }
    }

} // namespace gamo
