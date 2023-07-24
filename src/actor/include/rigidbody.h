#pragma once

#include "characteristic.h"
#include "physics.h"
#include "utils.h"
#include "event.h"

namespace gamo
{
    class RigidBody : public Characteristic
    {
    private:
        BodyDef _bodydef;
        PhysicsBody *_body = nullptr;

    public:
        RigidBody(Actor *actor) : Characteristic(actor) {}
        ~RigidBody();
        BodyDef &Define() { return _bodydef; }
        PhysicsBody *Body() { return _body; }

        EVENT_DECALRE(RigidBody, HandleInit, EVENT_RIGIDBODY_INIT);
        EVENT_DECALRE(RigidBody, HandleBeforeStep, EVENT_PHYSICS_BEFORE_STEP);
        EVENT_DECALRE(RigidBody, HandleAfterStep, EVENT_PHYSICS_AFTER_STEP);

        void RegisterEvents() override { RegisterHandleInit(); RegisterHandleBeforeStep(); RegisterHandleAfterStep(); }
        void UnregisterEvents() override { UnregisterHandleInit(); UnregisterHandleBeforeStep(); UnregisterHandleAfterStep(); }

        std::string Type() override { return std::string("RigidBody"); }
    };

    class RigidBodyEvent : public Event
    {
    private:
        PhysicsWorld *_world;
    public:
        RigidBodyEvent(PhysicsWorld *world) : _world(world) {}
        PhysicsWorld *World() { return _world; }

        virtual int Type() const override { return EVENT_RIGIDBODY_INIT; }
    };
} // namespace gamo
