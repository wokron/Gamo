#pragma once

#include "characteristic_base.h"
#include "render.h"
#include "camera.h"
#include "event.h"
#include "utils.h"
#include <string>

namespace gamo
{
    class Sprite;

    class Renderer : public Characteristic
    {
    private:
        Sprite *_target_sprite = nullptr;
        ColorAlpha _color_config = {255, 255, 255, 255};
        unsigned int _render_level = 0;
        bool _visiable = true;

    public:
        Renderer(Actor *actor) : Characteristic(actor) {}

        Sprite *TargetSprite() { return _target_sprite; }
        void TargetSprite(Sprite *s) { _target_sprite = s; }

        ColorAlpha ColorConfig() { return _color_config; }
        void ColorConfig(ColorAlpha coloralpha) { _color_config = coloralpha; }

        unsigned int RenderLevel() { return _render_level; }
        void RenderLevel(unsigned int render_level) { _render_level = render_level; }

        bool Visiable() { return _visiable; }
        void Visiable(bool visiable) { _visiable = visiable; }

        /// @brief render a sprite with the specified config in this "Renderer", if the sprite is null, will just not render
        /// @param position the position to render in window, top-left corner is the zero point. unit is *Unit*,
        /// note that this should be the relative position with camera, differ from the position of the actor
        /// @param window_pixel_per_unit relation between window's *Pixel* and *Unit*, this is for camera
        /// @return 0 on success, or < 0 on faliure
        int Render(SDL_FPoint *position, float window_pixel_per_unit);

        /// @brief render area is an aabb rect area that encloses the sprite
        SDL_FRect RenderArea();

        EVENT_DECALRE(Renderer, HandleCameraDetect, EVENT_CAMERA_DETECT);

        void RegisterEvents() override { RegisterHandleCameraDetect(); }
        void UnregisterEvents() override { UnregisterHandleCameraDetect(); }

        std::string Type() { return std::string("Renderer"); }

    private:
        SDL_FPoint DoRotate(SDL_FPoint vect, float angle);
        SDL_FPoint DoScale(SDL_FPoint vect, SDL_FPoint scale);
    };

    class RendererEvent : public Event
    {
    private:
        Camera *_camera;
        SDL_FRect *_view;

    public:
        RendererEvent(Camera *camera, SDL_FRect *view) : _camera(camera), _view(view) {}

        Camera *TargetCamera() { return _camera; }
        SDL_FRect *CameraView() { return _view; }

        int Type() const override { return EVENT_CAMERA_DETECT; }
    };
} // namespace gamo
