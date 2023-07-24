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
        _bodydef.userData.pointer = (uintptr_t)this;
        _body = world->CreateBody(&_bodydef);
        auto pos = GetTransform()->Position();
        auto angle = GetTransform()->Rotate() * M_PI / 180;
        _body->SetTransform({pos.x, pos.y}, angle);
        UnregisterHandleInit();
    }

    void RigidBody::HandleBeforeStep(Event *e)
    {
        auto transform = GetTransform();
        if (transform->IsPositionOrRotateModified())
        {
            auto tmp = transform->Position();
            Vec2 pos(tmp.x, tmp.y);
            _body->SetTransform(pos, transform->Rotate() * M_PI / 180);
        }
    }

    void RigidBody::HandleAfterStep(Event *e)
    {
        auto pos = _body->GetPosition();
        auto angle = _body->GetAngle();
        auto transform = GetTransform();
        transform->Position({pos.x, pos.y});
        transform->Rotate(angle / M_PI * 180);
        transform->IsPositionOrRotateModified();
    }
} // namespace gamo
