#include "gamo/game/play.h"
#include "gamo/director/director.h"
#include "gamo/render/render.h"
#include "gamo/game/scene.h"
#include "spdlog/spdlog.h"
#include "gamo/event/event.h"
#include "gamo/input/input.h"
#include <string>
#include <algorithm>

namespace gamo
{
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

            if (_new_scene != nullptr)
            {
                spdlog::info("switch to a new Scene[{}]", fmt::ptr(_new_scene));
                _new_scene->RegisterSystemEvents();
                _new_scene = nullptr;
            }

            Input::GetInstance()->UpdateInputStates();

            // init event will be unregistered after the first call, so this is not a bug
            cur_scene->ApplyInit();

            cur_scene->PhysicsStep(_target_frame_rate);

            cur_scene->LogicStep();

            if (cur_scene->RenderStep() < 0)
            {
                spdlog::error("fail to finish render step");
            }

            EventDispatcher::GetInstance()->Dispatch(EVENT_MEM_FREE, nullptr); // memory free

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
        while (!_scene_stack.empty())
        {
            PopScene();
        }
        EventDispatcher::GetInstance()->Dispatch(EVENT_MEM_FREE, nullptr); // memory free
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

        if (!_scene_stack.empty())
        {
            _scene_stack.top()->UnregisterSystemEvents();
        }

        _scene_stack.push(scene);
        _new_scene = scene;
    }

    void Play::PopScene()
    {
        if (!_scene_stack.empty())
        {
            auto scene = _scene_stack.top();
            _scene_stack.pop();
            scene->Deref();
        }
        _new_scene = CurrentScene();
    }

    Scene *Play::CurrentScene()
    {
        return _scene_stack.empty() ? nullptr : _scene_stack.top();
    }
} // namespace gamo
