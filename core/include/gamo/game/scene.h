#pragma once

#include <vector>
#include "gamo/physics/physics.h"
#include "gamo/resource/resource.h"
#include "spdlog/spdlog.h"

namespace gamo
{
    class Actor;
    class Camera;

    class Scene : public IResourceSet<Actor>, public IResource
    {
    private:
        std::vector<Actor *> _actors = std::vector<Actor *>();
        b2World *_physics_world = nullptr;
        ContactListener _contact_listener;

    public:
        Scene();

        void AddActor(Actor *actor);

        Actor *CreateActor(Vect position, float rotate, Vect scale);

        /// @brief apply init events
        void ApplyInit();

        /// @brief redraw the scene
        /// @return 0 on success, < 0 on fail
        int RenderStep();

        /// @brief do physical simulation in the scene
        /// @param frames frames per second
        void PhysicsStep(unsigned int frames);

        /// @brief handle game logic
        void LogicStep();

        /// @brief register system events in this scene
        /// system events include events in behavior, in camera, etc.
        void RegisterSystemEvents();

        /// @brief unregister system events in this scene
        /// system events include events in behavior, in camera, etc.
        void UnregisterSystemEvents();

        void RemoveResource(Actor *item) override;

        void Destroy() override;
        void Deref() override;
        
        OVERRIDE_HANDLE_MEM_FREE(Scene)
        {
            UnregisterHandleMemFree();
            spdlog::info("Scene[{}] is destroying...", fmt::ptr(this));
            delete this;
        }
    };

} // namespace gamo
