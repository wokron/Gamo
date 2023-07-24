#pragma once

#include "physics_types.h"
#include <vector>
#include <assert.h>

namespace gamo
{
    class PhysicsConfig
    {
    private:
        Vec2 _gravity = Vec2(0.0f, -10.0f);
        int _velocity_iterations = 8; // recommend number
        int _position_iterations = 3; // recommend number

        static PhysicsConfig *_instance;

    public:
        /// @brief get the singleton instance of PhysicsConfig
        static PhysicsConfig *GetInstance()
        {
            if (_instance == nullptr)
            {
                _instance = new PhysicsConfig();
            }
            return _instance;
        }

        Vec2 Gravity() { return _gravity; }
        void Gravity(Vec2 gravity) { _gravity = gravity; }

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
    };
} // namespace gamo
