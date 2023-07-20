#include "actor.h"
#include "assert.h"
#include "event.h"

namespace gamo
{
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

    void Camera::RenderEventListener(Event *e)
    {
        auto view = View();
        auto camera_event = RendererEvent(this, &view);
        EventDispatcher::GetInstance()->Dispatch(&camera_event);
    }

    void Camera::RegisterRenderEvent()
    {
        _camera_event_handle = EventDispatcher::GetInstance()->Append(EVENT_RENDER, MEMBER_METHOD(this, &Camera::RenderEventListener));
    }

    void Camera::UnregisterRenderEvent()
    {
        EventDispatcher::GetInstance()->Remove(EVENT_RENDER, _camera_event_handle);
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
