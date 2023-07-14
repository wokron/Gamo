#pragma once

#include <stack>
#include <vector>
#include <string>

class SDL_Window;
class SDL_Renderer;

namespace gamo
{
    class Scene;

    /// @brief Play is a singleton class which manage all the resources in game
    class Play
    {
    private:
        std::vector<Scene *> _scenes = std::vector<Scene *>();
        std::stack<Scene *> _scene_stack = std::stack<Scene *>();

        SDL_Window *_window = nullptr;
        SDL_Renderer *_renderer = nullptr;
        unsigned int _target_frame_rate = 0;
        bool _quit = false;

        static Play *_instance;

    public:
        /// @brief get the singleton instance of Play
        static Play *GetInstance()
        {
            if (_instance == nullptr)
            {
                _instance = new Play();
            }
            return _instance;
        }

        SDL_Window *Window() { return _window; }
        SDL_Renderer *Renderer() { return _renderer; }

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

        /// @brief add a scene to the Play
        /// @param scene 
        void AddScene(Scene *scene);

        /// @brief this function will replace the currrent scene with another scene with the specific index
        /// @param index the index of the scene, corresponding to the order in which the scene is added to the Play
        /// @return 0 on success, < 0 on fail (index out of range)
        int ReplaceScene(int index);

        /// @brief push another scene on the top of the scene stack
        /// @param index the index of the scene, corresponding to the order in which the scene is added to the Play
        /// @return 0 on success, < 0 on fail (index out of range)
        int PushScene(int index);

        /// @brief pop the current scene from the scene stack
        void PopScene();

        /// @brief the current scene is the scene on the top of the scene stack
        /// @return the current scene, or null if there has no scenes on the stack
        Scene *CurrentScene();

    private:
        Play() {}
        ~Play() {}
        Play(const Play &);
        Play &operator=(const Play &);
    };
} // namespace gamo
