#pragma once

#include <vector>
#include <assert.h>
#include "utils.h"

namespace gamo
{
    SINGLETON_BEGIN(PhysicsConfig)
    private:
        Vect _gravity{0.0f, -10.0f};
        int _velocity_iterations = 8; // recommend number
        int _position_iterations = 3; // recommend number

    public:
        Vect Gravity() { return _gravity; }
        void Gravity(Vect gravity) { _gravity = gravity; }

        int VelocityIter() { return _velocity_iterations; }
        void VelocityIter(int v_iter)
        {
            assert(v_iter > 0);
            _velocity_iterations = v_iter;
        }
        int PositionIter() { return _position_iterations; }
        void PositionIter(int p_iter)
        {
            assert(p_iter > 0);
            _position_iterations = p_iter;
        }

    private:
        PhysicsConfig() {}
    SINGLETON_END
} // namespace gamo
