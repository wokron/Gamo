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

    void Scene::RegisterCameras()
    {
        for (auto actor : _actors)
        {
            auto cameras = GetCameras(actor);
            for (auto camera : cameras)
            {
                RenderDirector::GetInstance()->RegisterCamera(camera);
            }
        }
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

    std::vector<Camera *> Scene::GetCameras(Actor *actor)
    {
        std::vector<Camera *> result;
        auto rt = actor->GetCharacteristicsByType("Camera");
        for (auto camera : rt)
        {
            result.push_back((Camera *)camera);
        }

        for (auto sub_actor : actor->SubActors())
        {
            auto sub_rt = GetCameras(sub_actor);
            result.insert(result.end(), sub_rt.begin(), sub_rt.end());
        }

        return result;
    }

} // namespace gamo
