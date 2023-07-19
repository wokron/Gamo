#include "play.h"
#include "director.h"
#include "render.h"
#include "scene.h"
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "spdlog/spdlog.h"

namespace gamo
{
    Play *Play::_instance = nullptr; // init static member

    void Play::TargetFrameRate(int target_frame_rate)
    {
        assert(target_frame_rate >= -1 && target_frame_rate != 0);

        if (target_frame_rate == -1) // default set fps = refresh rate
        {
            SDL_DisplayMode mode;
            if (SDL_GetCurrentDisplayMode(0, &mode) < 0)
            {
                _target_frame_rate = _target_frame_rate != 0 ? _target_frame_rate : 60;
                return;
            }
            _target_frame_rate = mode.refresh_rate != 0 ? mode.refresh_rate : 60;
        }
        else
        {
            _target_frame_rate = target_frame_rate;
        }
    }

    int Play::Init(std::string title, int w, int h, bool accelerate)
    {
        int r;
        if ((r = RenderAsset::GetInstance()->Init(title, w, h, accelerate)) < 0)
        {
            return r;
        }

        TargetFrameRate(-1);

        return 0;
    }

    void Play::Perform()
    {
        Scene *cur_scene;
        while (!_quit && (cur_scene = CurrentScene()) != nullptr)
        {
            auto target_ms = SDL_GetTicks64() + 1000u / _target_frame_rate;

            cur_scene->PhysicsStep();
            cur_scene->RenderStep();

            auto finish_ms = SDL_GetTicks64();
            if (target_ms > finish_ms)
            {
                SDL_Delay((unsigned int)(target_ms - finish_ms));
            }
        }
    }

    void Play::Destroy()
    {
        RenderAsset::GetInstance()->Destroy();
    }

    void Play::ReplaceScene(Scene *scene)
    {
        assert(scene != nullptr);

        PopScene();

        PushScene(scene);
    }

    void Play::PushScene(Scene *scene)
    {
        assert(scene != nullptr);

        _scene_stack.push(scene);
        RenderDirector::GetInstance()->UnRegisterAllCameras();
        scene->RegisterCameras();
    }

    void Play::PopScene()
    {
        if (!_scene_stack.empty())
        {
            auto scene = _scene_stack.top();
            _scene_stack.pop();
            delete scene; // free the scene, todo: there may have some bug
        }
    }

    Scene *Play::CurrentScene()
    {
        return _scene_stack.empty() ? nullptr : _scene_stack.top();
    }
} // namespace gamo
