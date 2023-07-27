#include "actor.h"
#include "game.h"
#include "collider.h"
#include "math.h"

namespace gamo
{
    RigidBody::~RigidBody()
    {  
        if (_body != nullptr)
        {
            auto world = _body->GetWorld();
            world->DestroyBody(_body);
        }
    }

    void RigidBody::HandleInit(Event *e)
    {
        auto event = (RigidBodyEvent *)e;
        auto world = event->World();

        // body definition for initial
        b2BodyDef bodydef;
        bodydef.position = GetTransform()->GlobalPosition();
        bodydef.angle = GetTransform()->GlobalRotate() * M_PI / 180;
        bodydef.linearVelocity = _linear_velocity;
        bodydef.angularVelocity = _angular_velocity;
        bodydef.linearDamping = _linear_damping;
        bodydef.angularDamping = _angular_damping;
        bodydef.allowSleep = _allow_sleep;
        bodydef.fixedRotation = _fixed_rotate;
        bodydef.bullet = _bullet;
        bodydef.type = _body_type;
        bodydef.gravityScale = _gravity_scale;
        bodydef.userData.pointer = (uintptr_t)this;

        _body = world->CreateBody(&bodydef);
        
        UnregisterHandleInit();
    }

    void RigidBody::HandleBeforeStep(Event *e)
    {
        auto transform = GetTransform();
        if (transform->IsPositionOrRotateModified())
        {
            auto pos = transform->Position();
            float angle = transform->Rotate() * M_PI / 180; 
            _body->SetTransform(pos, angle);
        }
    }

    void RigidBody::HandleAfterStep(Event *e)
    {
        auto pos = _body->GetPosition();
        auto angle = _body->GetAngle();
        auto transform = GetTransform();
        transform->Position({pos.x, pos.y});
        transform->Rotate(angle / M_PI * 180);
        transform->IsPositionOrRotateModified(); // reset flag
    }

    Vect RigidBody::LinearVelocity()
    {
        if (_body)
            _linear_velocity = Vect::FromVec2(_body->GetLinearVelocity());
        return _linear_velocity;
    }

    void RigidBody::LinearVelocity(Vect lv)
    {
        _linear_velocity = lv;
        if (_body)
            _body->SetLinearVelocity(_linear_velocity);
    }

    float RigidBody::AngularVelocity()
    {
        if (_body)
            _angular_velocity = _body->GetAngularVelocity();
        return _angular_velocity;
    }

    void RigidBody::AngularVelocity(float av)
    {
        _angular_velocity = av;
        if (_body)
            _body->SetAngularVelocity(_angular_velocity);
    }

    b2BodyType RigidBody::BodyType()
    {
        if (_body)
            _body_type = _body->GetType();
        return _body_type;
    }

    void RigidBody::BodyType(b2BodyType type)
    {
        _body_type = type;
        if (_body)
            _body->SetType(_body_type);
    }

    float RigidBody::LinearDamping()
    {
        if (_body)
            _linear_damping = _body->GetLinearDamping();
        return _linear_damping;
    }

    void RigidBody::LinearDamping(float ad)
    {
        _linear_damping = ad;
        if (_body)
            _body->SetLinearDamping(_linear_damping);
    }

    float RigidBody::AngularDamping()
    {
        if (_body)
            _angular_damping = _body->GetAngularDamping();
        return _angular_damping;
    }

    void RigidBody::AngularDamping(float ad)
    {
        _angular_damping = ad;
        if (_body)
            _body->SetAngularDamping(_angular_damping);
    }

    bool RigidBody::AllowSleep()
    {
        if (_body)
            _allow_sleep = _body->IsSleepingAllowed();
        return _allow_sleep;
    }

    void RigidBody::AllowSleep(bool allow)
    {
        _allow_sleep = allow;
        if (_body)
            _body->SetSleepingAllowed(_allow_sleep);
    }

    bool RigidBody::FixedRotate()
    {
        if (_body)
            _fixed_rotate = _body->IsFixedRotation();
        return _fixed_rotate;
    }

    void RigidBody::FixedRotate(bool fr)
    {
        _fixed_rotate = fr;
        if (_body)
            _body->SetFixedRotation(_fixed_rotate);
    }

    float RigidBody::GravityScale()
    {
        if (_body)
            _gravity_scale = _body->GetGravityScale();
        return _gravity_scale;
    }

    void RigidBody::GravityScale(float scale)
    {
        _gravity_scale = scale;
        if (_body)
            _body->SetGravityScale(_gravity_scale);
    }

    bool RigidBody::Bullet()
    {
        if (_body)
            _bullet = _body->IsBullet();
        return _bullet;
    }

    void RigidBody::Bullet(bool bullet)
    {
        _bullet = bullet;
        if (_body)
            _body->SetBullet(_bullet);
    }

} // namespace gamo
