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

} // namespace gamo
