#pragma once

#include "characteristic.h"
#include "physics.h"
#include "event.h"

#define COLLISION_CATEGORY(n) (1u << (n))

namespace gamo
{
    class Collider : public Characteristic
    {
    private:
        // b2FixtureDef _fixturedef;
        b2Fixture *_fixture = nullptr;

        Shape *_shape = nullptr;
        float _friction = 0.2f;
        float _restitution = 0.0f;
        float _density = 0.0f;
        bool _is_sensor = false;
        unsigned short _category = 0x0001;
        unsigned short _collide_with = 0xFFFF;

    public:
        Collider(Actor *actor) : Characteristic(actor) {}
        // b2FixtureDef &Define() { return _fixturedef; }
        b2Fixture *Fixture() { return _fixture; }

        Shape *ColliderShape();
        void ColliderShape(Shape *shape);

        float Friction();
        void Friction(float f);

        float Restitution();
        void Restitution(float r);

        float Density();
        void Density(float d);

        bool IsSensor();
        void IsSensor(bool is_sensor);

        unsigned short Category();
        void Category(unsigned short c);

        unsigned short CollideWith();
        void CollideWith(unsigned short c);

        EVENT_DECLARE(Collider, HandleInit, EVENT_COLLIDER_INIT);
        EVENT_DECLARE(Collider, HandleBeforeStep, EVENT_PHYSICS_BEFORE_STEP);

        void RegisterEvents() override { RegisterHandleInit(); RegisterHandleBeforeStep(); Characteristic::RegisterEvents(); }
        void UnregisterEvents() override { UnregisterHandleInit(); UnregisterHandleBeforeStep(); Characteristic::UnregisterEvents(); }

        void DispatchCollisionBegin(std::vector<Collider *> &others);
        void DispatchCollisionEnd(std::vector<Collider *> &others);

        std::string Type() override { return std::string("Collider"); }
    
    private:
        void CreateAndRepalceFixture(b2Body *rigidbody, Vect offset, float rotate, Vect scale);
        RigidBody *SearchRigidBody(Vect *rt_offset, float *rt_rotate, Vect *rt_scale);
    };

} // namespace gamo
