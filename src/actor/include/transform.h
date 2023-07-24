#pragma once

#include "utils.h"
#include "characteristic_base.h"
#include <string>

namespace gamo
{
    class Actor;

    class Transform : public Characteristic
    {
    private:
        FPoint _position;
        float _rotate;
        FVect _scale;
        bool _is_pos_rotate_modified = false;
        bool _is_scale_modified = true;

    public:
        Transform(Actor *actor, FPoint position, float rotate, FVect scale)
            : Characteristic(actor), _position(position), _rotate(rotate), _scale(scale) {}

        FPoint Position() { return _position; }
        void Position(FPoint position) { _position = position; _is_pos_rotate_modified = true; }

        float Rotate() { return _rotate; }
        void Rotate(float rotate) { _rotate = rotate; _is_pos_rotate_modified = true; }

        FVect Scale() { return _scale; }
        void Scale(FVect scale) { _scale = scale; _is_scale_modified = true; }

        bool IsPositionOrRotateModified() { auto tmp = _is_pos_rotate_modified; _is_pos_rotate_modified = false; return tmp; }
        bool IsScaleModified() { auto tmp = _is_scale_modified; _is_scale_modified = false; return tmp; }

        FPoint GlobalPosition();
        float GlobalRotate();
        FPoint GlobalScale();

        std::string Type() { return std::string("Transform"); }
    };
} // namespace gamo
