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

    public:
        Transform(Actor *actor, FPoint position, float rotate, FVect scale)
            : Characteristic(actor), _position(position), _rotate(rotate), _scale(scale) {}

        FPoint Position() { return _position; }
        void Position(FPoint position) { _position = position; }

        float Rotate() { return _rotate; }
        void Rotate(float rotate) { _rotate = rotate; }

        FVect Scale() { return _scale; }
        void Scale(FVect scale) { _scale = scale; }

        std::string Type() { return std::string("Transform"); }
    };
} // namespace gamo
