#pragma once

#include <vector>
#include <queue>
#include "utils.h"

namespace gamo
{
    class RenderCall;
    class Renderer;
    class Camera;
    class Actor;

    /// @brief RenderDirector is a singleton class which manage the render procedure of the game
    class RenderDirector
    {
    private:
        std::vector<Camera *> _cameras = std::vector<Camera *>();
        std::priority_queue<RenderCall> _render_queue = std::priority_queue<RenderCall>();

        static RenderDirector *_instance;

    public:
        /// @brief get the singleton instance of RenderDirector
        /// @param camera
        static RenderDirector *GetInstance()
        {
            if (_instance == nullptr)
            {
                _instance = new RenderDirector();
            }
            return _instance;
        }

        /// @brief register a camera to the RenderDirector, 
        /// only registered cameras will be rendered
        /// @param camera the camera to register
        void RegisterCamera(Camera *camera);

        /// @brief unregister all the cameras in RenderDirector
        void UnRegisterAllCameras();

        /// @brief this function will detect all the Renderer characteristic in actors, 
        /// build render-calls and push them to the render queue. 
        /// so should to be called before Render()
        /// @param actors the actors to detect
        void DetectRenderer(std::vector<Actor *> actors);

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
        ~RenderDirector() {}
        RenderDirector(const RenderDirector &);
        RenderDirector &operator=(const RenderDirector &);

        void QueueClear(std::priority_queue<RenderCall> &q);
    };

    class RenderCall
    {
    private:
        Renderer *_renderer;
        FPoint _position;
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
