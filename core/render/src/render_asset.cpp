#include "render_asset.h"
#include "spdlog/spdlog.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

namespace gamo
{
    RenderAsset *RenderAsset::_instance = nullptr; // init static member

    int RenderAsset::Init(std::string title, int w, int h, bool accelerate)
    {
        int r;
        if ((r = SDL_Init(SDL_INIT_EVERYTHING)) < 0)
        {
            spdlog::error("fail to init SDL, {}", SDL_GetError());
            return r;
        }

        if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP) != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP))
        {
            spdlog::error("fail to init SDL_Image, {}", IMG_GetError());
            return -1;
        }

        _window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            w, h,
            SDL_WINDOW_SHOWN);
        if (_window == nullptr)
        {
            spdlog::error("fail to create window, {}", SDL_GetError());
            return -1;
        }

        _renderer = SDL_CreateRenderer(
            _window, -1,
            accelerate ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE);
        if (_renderer == nullptr)
        {
            spdlog::error("fail to create renderer, {}", SDL_GetError());
            return -1;
        }

        return 0;
    }

    void RenderAsset::Destroy()
    {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        _renderer = nullptr;
        _window = nullptr;
    }
} // namespace gamo
