#pragma once

#include "scene.h"
#include "gamo/utils/singleton.h"
#include "gamo/utils/singleton.h"
#include <stack>
#include <vector>
#include <string>

class SDL_Window;
class SDL_Renderer;

namespace gamo
{
    class Scene;

    /// @brief Play is a singleton class which manage all the resources in game
    SINGLETON_BEGIN(Play)
    private:
        std::stack<Scene *> _scene_stack = std::stack<Scene *>();
        Scene *_new_scene = nullptr;

        unsigned int _target_frame_rate = 0;
        bool _quit = false;

    public:
        /// @brief target frame rate is the expected frame rate to refresh the game 
        unsigned int TargetFrameRate() { return _target_frame_rate; }

        /// @brief set the target frame rate, which is the expected frame rate to refresh the game 
        /// @param target_frame_rate the target frame rate should be > 0, or -1 for default frame rate
        void TargetFrameRate(int target_frame_rate);

        bool Quit() { return _quit; }
        void Quit(bool quit) { _quit = quit; }

        /// @brief initial the Play, as well as the game
        /// @param title the title of the window
        /// @param w the initial width of the window
        /// @param h the initial height of the window
        /// @param accelerate if use hardware to accelerate the render
        /// @return 0 on success or < 0 on failure
        int Init(std::string title, int w, int h, bool accelerate);

        /// @brief perform the play (run the game)
        /// Post-condition: the Play should be initialized successfully
        void Perform();

        /// @brief free the resources in Play
        void Destroy();

        /// @brief this function will replace the currrent scene with another scene with the specific index
        /// @param scene the scene to replace the current scene, and the current scene will be freed
        void ReplaceScene(Scene *scene);

        /// @brief push another scene on the top of the scene stack
        /// @param scene the scene to cover the current scene, the scenes behind will not be freed
        void PushScene(Scene *scene);

        /// @brief pop the current scene from the scene stack, 
        /// and free the resources of the scene, 
        /// if there isn't any scenes now, just do nothing
        void PopScene();

        /// @brief the current scene is the scene on the top of the scene stack
        /// @return the current scene, or null if there has no scenes on the stack
        Scene *CurrentScene();

    private:
        Play() {}
    SINGLETON_END
} // namespace gamo
