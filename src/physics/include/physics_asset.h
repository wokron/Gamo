#pragma once

#include "physics_types.h"
#include <vector>

namespace gamo
{
    class PhysicsAsset
    {
    private:
        Vec2 _gravity = Vec2(0.0f, -10.0f);

        static PhysicsAsset *_instance;
    public:
        /// @brief get the singleton instance of PhysicsAsset
        static PhysicsAsset *GetInstance()
        {
            if (_instance == nullptr)
            {
                _instance = new PhysicsAsset();
            }
            return _instance;
        }

        Vec2 Gravity() { return _gravity; }
        void Gravity(Vec2 gravity) { _gravity = gravity; }
    };
} // namespace gamo
