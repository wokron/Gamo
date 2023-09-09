#include <SDL2/SDL.h>
#include <string>
#include "gamo/utils/utils.h"

namespace gamo
{
    SINGLETON_BEGIN(RenderAsset)
    private:
        SDL_Window *_window;
        SDL_Renderer *_renderer;
        
    public:
        /// @brief initial the RenderAsset
        /// @param title the title of the window
        /// @param w the initial width of the window
        /// @param h the initial height of the window
        /// @param accelerate if use hardware to accelerate the render
        /// @return 0 on success or < 0 on failure
        int Init(std::string title, int w, int h, bool accelerate);

        /// @brief free the resources in RenderAsset
        void Destroy();

        SDL_Window *Window() { return _window; }
        SDL_Renderer *Renderer() { return _renderer; }

    private:
        RenderAsset() {}
    SINGLETON_END

} // namespace gamo
