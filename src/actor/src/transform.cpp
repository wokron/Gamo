#include "actor.h"
#include "math.h"

namespace gamo
{
    Vect Transform::GlobalPosition()
    {
        auto sup_actor = BelongActor()->SupActor();
        if (sup_actor == nullptr)
        {
            return Position();
        }
        else
        {
            auto sup_pos = sup_actor->GetTransform()->GlobalPosition();
            auto sup_angle = sup_actor->GetTransform()->Rotate();
            auto relative_pos = Position();
            Matrix rotate_m; rotate_m.AsRotate((sup_angle * M_PI / 180));
            return rotate_m * relative_pos + sup_pos;
        }
    }

    float Transform::GlobalRotate()
    {
        auto sup_actor = BelongActor()->SupActor();
        if (sup_actor == nullptr)
        {
            return Rotate();
        }
        else
        {
            auto sup_rotate = sup_actor->GetTransform()->GlobalRotate();
            return sup_rotate + Rotate();
        }
    }

    Vect Transform::GlobalScale()
    {
        auto sup_actor = BelongActor()->SupActor();
        if (sup_actor == nullptr)
        {
            return Scale();
        }
        else
        {
            auto sup_scale = sup_actor->GetTransform()->GlobalScale();
            auto relative_scale = Scale();
            return relative_scale * sup_scale;
        }
    }

} // namespace gamo
