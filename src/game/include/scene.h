#pragma once

#include <vector>
#include "physics.h"

namespace gamo
{
    class Actor;
    class Camera;

    class Scene
    {
    private:
        std::vector<Actor *> _actors = std::vector<Actor *>();
        PhysicsWorld _physics_world = PhysicsWorld(PhysicsAsset::GetInstance()->Gravity());

    public:
        void AddActor(Actor *actor);

        /// @brief redraw the scene
        /// @return 0 on success, < 0 on fail
        int RenderStep();

        /// @brief do physical simulation in the scene
        void PhysicsStep();

        /// @brief handle game logic
        void LogicStep();

        /// @brief register system events in this scene
        /// system events include events in behavior, in camera, etc.
        void RegisterSystemEvents();

        /// @brief unregister system events in this scene
        /// system events include events in behavior, in camera, etc.
        void UnregisterSystemEvents();
    };

} // namespace gamo
