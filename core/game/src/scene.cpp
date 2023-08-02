#include "scene.h"
#include "director.h"
#include "actor.h"
#include "spdlog/spdlog.h"
#include <algorithm>

namespace gamo
{
    Scene::Scene()
    {
        _physics_world = new b2World(PhysicsConfig::GetInstance()->Gravity());
        _physics_world->SetContactListener(&_contact_listener);
    }

    void Scene::AddActor(Actor *actor)
    {
        _actors.push_back(actor);
    }

    Actor* Scene::CreateActor(Vect position, float rotate, Vect scale)
    {
        auto actor = new Actor(this, position, rotate, scale);
        AddActor(actor);
        return actor;
    }

    void Scene::ApplyInit()
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_START, nullptr);
        RigidBodyEvent e(_physics_world);
        EventDispatcher::GetInstance()->Dispatch(&e); // init rigidbody
        EventDispatcher::GetInstance()->Dispatch(EVENT_COLLIDER_INIT, nullptr); // collider must initialed after rigidbody
    }

    int Scene::RenderStep()
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_RENDER, nullptr);
        return RenderDirector::GetInstance()->Render();
    }

    void Scene::PhysicsStep(unsigned int frames)
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_PHYSICS_BEFORE_STEP, nullptr);

        auto config = PhysicsConfig::GetInstance();
        _physics_world->Step(1.0f / frames, config->VelocityIter(), config->PositionIter()); // do physics simulation

        EventDispatcher::GetInstance()->Dispatch(EVENT_PHYSICS_AFTER_STEP, nullptr);

        // now dispatch collision events
        for (auto& pair : _contact_listener.BeginContactBuffer())
        {
            pair.first->DispatchCollisionBegin(pair.second);
            assert(pair.second.empty());
        }
        for (auto& pair : _contact_listener.EndContactBuffer())
        {
            pair.first->DispatchCollisionEnd(pair.second);
            assert(pair.second.empty());
        }
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

    void Scene::RemoveResource(Actor *item)
    {
        auto find = std::find(_actors.begin(), _actors.end(), item);
        if (find == _actors.end())
            return;

        _actors.erase(find);
        item->Deref();
    }

    void Scene::Destroy()
    {
        spdlog::warn("Scene cannot be destroyed through Destroy()");
    }

    void Scene::Deref()
    {
        UnregisterSystemEvents();
        RegisterHandleMemFree();
        for (auto item : _actors)
        {
            RemoveResource(item);
        }
    }

} // namespace gamo
