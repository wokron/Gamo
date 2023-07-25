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
        SDL_FPoint _position;
        float _rotate;
        SDL_FPoint _scale;
        bool _is_pos_rotate_modified = false;
        bool _is_scale_modified = true;

    public:
        Transform(Actor *actor, SDL_FPoint position, float rotate, SDL_FPoint scale)
            : Characteristic(actor), _position(position), _rotate(rotate), _scale(scale) {}

        SDL_FPoint Position() { return _position; }
        void Position(SDL_FPoint position) { _position = position; _is_pos_rotate_modified = true; }

        float Rotate() { return _rotate; }
        void Rotate(float rotate) { _rotate = rotate; _is_pos_rotate_modified = true; }

        SDL_FPoint Scale() { return _scale; }
        void Scale(SDL_FPoint scale) { _scale = scale; _is_scale_modified = true; }

        bool IsPositionOrRotateModified() { auto tmp = _is_pos_rotate_modified; _is_pos_rotate_modified = false; return tmp; }
        bool IsScaleModified() { auto tmp = _is_scale_modified; _is_scale_modified = false; return tmp; }

        SDL_FPoint GlobalPosition();
        float GlobalRotate();
        SDL_FPoint GlobalScale();

        std::string Type() { return std::string("Transform"); }
    };
} // namespace gamo
