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
        b2Body *_body = nullptr;

        Vect _linear_velocity = {0, 0};
        float _angular_velocity = 0;
        b2BodyType _body_type = b2_staticBody;
        float _linear_damping = 0;
        float _angular_damping = 0;
        bool _allow_sleep = 0;
        bool _fixed_rotate = false;
        float _gravity_scale = 1;
        bool _bullet = false;

    public:
        RigidBody(Actor *actor) : Characteristic(actor) {}
        ~RigidBody();
        b2Body *Body() { return _body; }

        Vect LinearVelocity();
        void LinearVelocity(Vect lv);

        float AngularVelocity();
        void AngularVelocity(float av);

        b2BodyType BodyType();
        void BodyType(b2BodyType type);

        float LinearDamping();
        void LinearDamping(float ad);

        float AngularDamping();
        void AngularDamping(float ad);

        bool AllowSleep();
        void AllowSleep(bool allow);

        bool FixedRotate();
        void FixedRotate(bool fr);

        float GravityScale();
        void GravityScale(float scale);

        bool Bullet();
        void Bullet(bool bullet);

        EVENT_DECLARE(RigidBody, HandleInit, EVENT_RIGIDBODY_INIT);
        EVENT_DECLARE(RigidBody, HandleBeforeStep, EVENT_PHYSICS_BEFORE_STEP);
        EVENT_DECLARE(RigidBody, HandleAfterStep, EVENT_PHYSICS_AFTER_STEP);

        void RegisterEvents() override { RegisterHandleInit(); RegisterHandleBeforeStep(); RegisterHandleAfterStep(); }
        void UnregisterEvents() override { UnregisterHandleInit(); UnregisterHandleBeforeStep(); UnregisterHandleAfterStep(); }

        std::string Type() override { return std::string("RigidBody"); }
    };

    class RigidBodyEvent : public Event
    {
    private:
        b2World *_world;
    public:
        RigidBodyEvent(b2World *world) : _world(world) {}
        b2World *World() { return _world; }

        virtual int Type() const override { return EVENT_RIGIDBODY_INIT; }
    };
} // namespace gamo
