#pragma once

#include <vector>
#include <queue>
#include "gamo/utils/utils.h"

namespace gamo
{
    class RenderCall;
    class Renderer;
    class Camera;
    class Actor;

    /// @brief RenderDirector is a singleton class which manage the render procedure of the game
    SINGLETON_BEGIN(RenderDirector)
    private:
        std::priority_queue<RenderCall> _render_queue = std::priority_queue<RenderCall>();

    public:
        /// @brief create a render-call and push it into the render queue
        /// @param renderer the renderer to create a render-call
        /// @param camera the camera to create a render-call
        void PushRenderCall(Renderer *renderer, Camera *camera);

        /// @brief execute render-call in the render queue,
        /// this function will clear the render queue
        /// Post-condition: function DetectRenderer() need to be called
        /// @return 0 on success, < 0 on error while rendering,
        /// but this function will always call all the render-call in render queue
        int Render();

    private:
        RenderDirector() {}
    SINGLETON_END

    class RenderCall
    {
    private:
        Renderer *_renderer;
        Vect _position;
        float _wppu;
        unsigned long long _order;

    public:
        RenderCall(Renderer *renderer, Camera *camera);

        /// @brief execute the render-call
        int Invoke();

        // means that the render-call with the smaller order will be called earlier
        bool operator<(const RenderCall &t) const
        {
            return _order > t._order; // min heap
        }
    };

} // namespace gamo
