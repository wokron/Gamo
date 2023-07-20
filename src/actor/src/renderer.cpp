#include "actor.h"
#include "render.h"
#include "director.h"

namespace gamo
{
    int Renderer::Render(FPoint *position, float window_pixel_per_unit)
    {
        if (_target_sprite == nullptr || !_visiable)
        {
            return 0;
        }

        Transform *transform = GetTransform();

        auto scale = transform->Scale();
        int r = _target_sprite->Render(position, transform->Rotate(), &scale, &_color_config, window_pixel_per_unit);
        if (r < 0)
        {
            return r;
        }

        return 0;
    }

    FRect Renderer::RenderArea()
    {
        float sprite_width = _target_sprite->UnitWidth();
        float sprite_height = _target_sprite->UnitHeight();
        auto transform = GetTransform();
        FPoint pivot = _target_sprite->Pivot();
        float pivot_w = pivot.x * sprite_width;
        float pivot_h = pivot.y * sprite_height;

        FVect corners[4] = {
            {-pivot_w, pivot_h},                                  // left-top
            {sprite_width - pivot_w, pivot_h},                    // right-top
            {-pivot_w, -(sprite_height - pivot_h)},               // left-bottom
            {sprite_width - pivot_w, -(sprite_height - pivot_h)}, // right-bottom
        };

        auto pos = transform->Position();
        for (int i = 0; i < 4; i++)
        {
            corners[i] = DoRotate(DoScale(corners[i], transform->Scale()), transform->Rotate());
            corners[i] = {pos.x + corners[i].x, pos.y + corners[i].y};
        }

        float minx = INFINITY, miny = INFINITY, maxx = -INFINITY, maxy = -INFINITY;
        for (int i = 0; i < 4; i++)
        {
            minx = std::min(minx, corners[i].x);
            miny = std::min(miny, corners[i].y);
            maxx = std::max(maxx, corners[i].x);
            maxy = std::max(maxy, corners[i].y);
        }

        return {minx, miny, maxx - minx, maxy - miny};
    }

    void Renderer::CameraDetectListener(Event *e)
    {
        auto event = (RendererEvent *)e;
        auto camera = event->TargetCamera();
        auto camera_view = event->CameraView();
        auto render_area = RenderArea();

        if (((BelongActor()->Layer() & camera->Layers()) != 0) && Visiable() && SDL_HasIntersectionF(camera_view, &render_area))
        {
            RenderDirector::GetInstance()->PushRenderCall(this, camera);
        }
    }

    void Renderer::RegisterCameraDetectEvent()
    {
        _renderer_event_handle = EventDispatcher::GetInstance()->Append(EVENT_CAMERA_DETECT, MEMBER_METHOD(this, &Renderer::CameraDetectListener));
    }

    void Renderer::UnregisterCameraDetectEvent()
    {
        EventDispatcher::GetInstance()->Remove(EVENT_CAMERA_DETECT, _renderer_event_handle);
    }

    FVect Renderer::DoRotate(FVect vect, float angle)
    {
        float theta = angle * M_PI / 180;
        float x = vect.x * cosf(theta) - vect.y * sinf(theta);
        float y = vect.x * sinf(theta) + vect.y * cosf(theta);
        return {x, y};
    }

    FVect Renderer::DoScale(FVect vect, FVect scale)
    {
        float x = vect.x * scale.x;
        float y = vect.y * scale.y;
        return {x, y};
    }
} // namespace gamo
