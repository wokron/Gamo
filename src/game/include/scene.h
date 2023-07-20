#pragma once

#include <vector>

namespace gamo
{
    class Actor;
    class Camera;

    class Scene
    {
    private:
        std::vector<Actor *> _actors = std::vector<Actor *>();

    public:
        void AddActor(Actor *actor);

        /// @brief redraw the scene
        /// @return 0 on success, < 0 on fail
        int RenderStep();

        /// @brief do physical simulation in the scene
        void PhysicsStep();

        /// @brief handle game logic
        void LogicStep();

        /// @brief register all the cameras in the scene to RenderDirector
        void RegisterCameras();

        /// @brief register system events in this scene
        /// system events include events in behavior, in camera, etc.
        void RegisterSystemEvents();

        /// @brief unregister system events in this scene
        /// system events include events in behavior, in camera, etc.
        void UnregisterSystemEvents();

    private:
        std::vector<Camera *> GetCameras(Actor *actor);
    };

} // namespace gamo
