#include "render_director.h"
#include "characteristic.h"
#include "assert.h"

namespace gamo
{
    void RenderDirector::PushRenderCall(Renderer *renderer, Camera *camera)
    {
        _render_queue.emplace(renderer, camera);
    }

    int RenderDirector::Render()
    {
        auto renderer = RenderAsset::GetInstance()->Renderer();
        assert(renderer != nullptr);
        SDL_RenderClear(renderer);

        int r = 0;
        while (!_render_queue.empty())
        {
            auto rendercall = _render_queue.top();
            _render_queue.pop();

            if (rendercall.Invoke() < 0)
            {
                r = -1;
            }
        }

        SDL_RenderPresent(renderer);

        return r;
    }

    RenderCall::RenderCall(Renderer *renderer, Camera *camera)
    {
        _renderer = renderer;

        int win_width, win_height;
        auto window = RenderAsset::GetInstance()->Window();
        assert(window != nullptr);
        SDL_GetWindowSize(window, &win_width, &win_height);

        float h_size = camera->CameraSize();
        float w_size = h_size / win_height * win_width;
        Vect center_pos = {w_size, h_size};

        _wppu = win_height / (2 * h_size);

        auto render_pos = renderer->GetTransform()->GlobalPosition();
        auto camera_pos = camera->GetTransform()->GlobalPosition();
        auto relative_pos = render_pos - camera_pos;
        relative_pos.y = -relative_pos.y; // from world coordinate to window coordinate, y axies need to reverse.

        _position = center_pos + relative_pos;

        _order = (((unsigned long long)camera->Depth()) << 32) | ((unsigned long long)renderer->RenderLevel());
    }

    int RenderCall::Invoke()
    {
        return _renderer->Render(&_position, _wppu);
    }

} // namespace gamo
