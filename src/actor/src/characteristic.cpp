#include "actor.h"
#include "render.h"
#include "assert.h"
#include <math.h>

namespace gamo
{
    std::vector<Characteristic *> &Characteristic::GetCharacteristics()
    {
        return _belong_actor->GetCharacteristics();
    }

    Transform *Characteristic::GetTransform()
    {
        return _belong_actor->GetTransform();
    }

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

    std::vector<Renderer *> Camera::DetectRenderersInCameraView(std::vector<Actor *> candidate_actors)
    {
        std::vector<Renderer *> renderers_in_view = std::vector<Renderer *>();

        FRect camera_view = View();
        for (auto actor : candidate_actors)
        {
            auto renderer = (Renderer *)actor->GetCharacteristicByType("Renderer");
            // if the actor is not in camera's layers or doesn't have render or the render is invisiable
            if ((actor->Layer() & _layers) == 0 || renderer == nullptr || !renderer->Visiable())
            {
                continue;
            }

            FRect render_area = renderer->RenderArea();

            if (SDL_HasIntersectionF(&camera_view, &render_area))
            {
                renderers_in_view.push_back(renderer);
            }
        }

        return renderers_in_view;
    }

    FRect Camera::View()
    {
        int win_width, win_height;
        auto window = RenderAsset::GetInstance()->Window();
        assert(window != nullptr);
        SDL_GetWindowSize(window, &win_width, &win_height);

        float half_height = _camera_size;
        float half_width = half_height / win_height * win_width;
        auto transform = GetTransform();

        FRect view = {
            transform->Position().x - half_width,
            transform->Position().y - half_height,
            2 * half_width,
            2 * half_height};
        return view;
    }
} // namespace gamo
