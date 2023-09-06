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
        Vect _position;
        float _rotate;
        Vect _scale;

    public:
        Transform(Actor *actor, Vect position, float rotate, Vect scale)
            : Characteristic(actor), _position(position), _rotate(rotate), _scale(scale) {}

        Vect Position() { return _position; }
        void Position(Vect position) { _position = position; }

        /// @brief rotate for the actor, > 0 for counterclockwise
        float Rotate() { return _rotate; }
        void Rotate(float rotate) { _rotate = rotate; }

        Vect Scale() { return _scale; }
        void Scale(Vect scale) { _scale = scale; }


        Vect GlobalPosition();
        float GlobalRotate();
        Vect GlobalScale();

        std::string ToString()
        {
            return "<pos: " + _position.ToString() + ", rotate: " + std::to_string(_rotate) + ", scale: " + _scale.ToString() + ">";
        }

        std::string Type() { return std::string("Transform"); }

        Transform *Clone() override;
    };
} // namespace gamo
