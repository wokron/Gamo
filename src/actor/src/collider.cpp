#include "actor.h"
#include "collider.h"
#include "assert.h"

namespace gamo
{
    void Collider::HandleInit(Event *e)
    {
        auto rigidbody = (RigidBody *)BelongActor()->GetCharacteristicByType("RigidBody");
        if (rigidbody != nullptr)
        {
            assert(rigidbody->Body() != nullptr);
            _fixturedef.userData.pointer = (uintptr_t)this;
            _fixture = rigidbody->Body()->CreateFixture(&_fixturedef);
        }
        UnregisterHandleInit();
    }

    void Collider::HandleBeforeStep(Event *e)
    {
        auto transform = GetTransform();
        if (transform->IsScaleModified())
        {
            // todo: this part should be used to scale the collider, but I haven't thought of a good solution yet
        }
    }

    void Collider::DispatchCollisionBegin(std::vector<Collider *> &others)
    {
        auto behaviors = BelongActor()->GetCharacteristicsByType("Behavior");
        while (!others.empty())
        {
            for (auto item : behaviors)
            {
                auto behavior = (Behavior *)item;
                behavior->OnCollisionBegin(others.back());
            }
            others.pop_back();
        }
    }

    void Collider::DispatchCollisionEnd(std::vector<Collider *> &others)
    {
        auto behaviors = BelongActor()->GetCharacteristicsByType("Behavior");
        while (!others.empty())
        {
            for (auto item : behaviors)
            {
                auto behavior = (Behavior *)item;
                behavior->OnCollisionEnd(others.back());
            }
            others.pop_back();
        }
    }

} // namespace gamo
