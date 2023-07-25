#include "actor.h"
#include "assert.h"
#include "event.h"

namespace gamo
{
    void Camera::HandleRender(Event *e)
    {
        auto view = View();
        auto camera_event = RendererEvent(this, &view);
        EventDispatcher::GetInstance()->Dispatch(&camera_event);
    }

    Rect Camera::View()
    {
        int win_width, win_height;
        auto window = RenderAsset::GetInstance()->Window();
        assert(window != nullptr);
        SDL_GetWindowSize(window, &win_width, &win_height);

        float half_height = _camera_size;
        float half_width = half_height / win_height * win_width;
        auto pos = GetTransform()->GlobalPosition();

        Rect view = {
            pos.x - half_width,
            pos.y - half_height,
            2 * half_width,
            2 * half_height};
        return view;
    }
} // namespace gamo
