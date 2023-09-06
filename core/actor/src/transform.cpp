#include "actor.h"
#include "math.h"

namespace gamo
{
    Vect Transform::GlobalPosition()
    {
        Vect global_pos = {0, 0};
        auto actor = BelongActor();
        while (actor != nullptr)
        {
            float sup_angle = actor->SupActor() == nullptr ? 0 : actor->SupActor()->GetTransform()->Rotate();
            Matrix rotate_m; rotate_m.AsRotate((sup_angle * M_PI / 180));
            global_pos = global_pos + rotate_m * actor->GetTransform()->Position();
            actor = actor->SupActor();
        }
        return global_pos;
    }

    float Transform::GlobalRotate()
    {
        auto global_rotate = 0;
        auto actor = BelongActor();
        while (actor != nullptr)
        {
            global_rotate += actor->GetTransform()->Rotate();
            actor = actor->SupActor();
        }
        return global_rotate;
    }

    Vect Transform::GlobalScale()
    {
        Vect global_scale = {1, 1};
        auto actor = BelongActor();
        while (actor != nullptr)
        {
            global_scale = global_scale * actor->GetTransform()->Scale();
            actor = actor->SupActor();
        }
        return global_scale;
    }

    Transform *Transform::Clone()
    {
        auto obj = new Transform(nullptr, _position, _rotate, _scale);
        return obj;
    }

} // namespace gamo
