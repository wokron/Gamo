#include "actor.h"
#include "render.h"
#include "director.h"

namespace gamo
{
    int Renderer::Render(Vect *position, float window_pixel_per_unit)
    {
        if (_target_sprite == nullptr || !_visiable)
        {
            return 0;
        }

        Transform *transform = GetTransform();

        auto scale = transform->GlobalScale();
        auto rotate = -transform->GlobalRotate(); // > 0 for counterclockwise in transform, but for clockwise in Sprite::Render, so negative is needed
        int r = _target_sprite->Render(position, rotate, &scale, &_color_config, window_pixel_per_unit);
        if (r < 0)
        {
            return r;
        }

        return 0;
    }

    Rect Renderer::RenderArea()
    {
        float sprite_width = _target_sprite->UnitWidth();
        float sprite_height = _target_sprite->UnitHeight();
        auto transform = GetTransform();
        Vect pivot = _target_sprite->Pivot();
        Vect pivot_wh = pivot * Vect{sprite_width, sprite_height};
        float pivot_w = pivot_wh.x;
        float pivot_h = pivot_wh.y;

        Vect corners[4] = {
            {-pivot_w, pivot_h},                                  // left-top
            {sprite_width - pivot_w, pivot_h},                    // right-top
            {-pivot_w, -(sprite_height - pivot_h)},               // left-bottom
            {sprite_width - pivot_w, -(sprite_height - pivot_h)}, // right-bottom
        };

        auto pos = transform->GlobalPosition();
        auto scale = transform->GlobalScale();
        auto angle = transform->GlobalRotate();
        Matrix scale_m, rotate_m;
        scale_m.AsScale(scale), rotate_m.AsRotate(angle * M_PI / 180);

        for (auto &corner : corners)
        {
            corner = rotate_m * (scale_m * corner) + pos;
        }

        float minx = INFINITY, miny = INFINITY, maxx = -INFINITY, maxy = -INFINITY;
        for (auto corner : corners)
        {
            minx = std::min(minx, corner.x);
            miny = std::min(miny, corner.y);
            maxx = std::max(maxx, corner.x);
            maxy = std::max(maxy, corner.y);
        }

        return {minx, miny, maxx - minx, maxy - miny};
    }

    void Renderer::HandleCameraDetect(Event *e)
    {
        auto event = (RendererEvent *)e;
        auto camera = event->TargetCamera();
        SDL_FRect camera_view = *(event->CameraView());
        SDL_FRect render_area = RenderArea();

        if (((BelongActor()->Layer() & camera->Layers()) != 0) && Visiable() && SDL_HasIntersectionF(&camera_view, &render_area))
        {
            RenderDirector::GetInstance()->PushRenderCall(this, camera);
        }
    }
} // namespace gamo
