#include "actor.h"
#include "math.h"

namespace gamo
{
    FPoint Transform::GlobalPosition()
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
            float theta = sup_angle * M_PI / 180;
            float relative_pos_x_rotated = relative_pos.x * cosf(theta) - relative_pos.y * sinf(theta);
            float relative_pos_y_rotated = relative_pos.x * sinf(theta) + relative_pos.y * cosf(theta);
            return {sup_pos.x + relative_pos_y_rotated, sup_pos.y + relative_pos_x_rotated};
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

    FPoint Transform::GlobalScale()
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
            return {sup_scale.x * relative_scale.x, sup_scale.y * relative_scale.y};
        }
    }

} // namespace gamo
