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
        Vect halfwh = {half_width, half_height};
        auto pos = GetTransform()->GlobalPosition();

        Rect view = Rect::FromVect(pos - halfwh, halfwh * 2);
        return view;
    }

    Camera *Camera::Clone()
    {
        auto obj = new Camera(nullptr);
        obj->_camera_size = _camera_size;
        obj->_view = _view;
        obj->_depth = _depth;
        obj->_layers = _layers;
        return obj;
    }
} // namespace gamo
