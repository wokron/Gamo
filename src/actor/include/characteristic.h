#pragma once

#include <vector>
#include "utils.h"
#include "render.h"

namespace gamo
{
    class Actor;
    class Transform;

    class Characteristic
    {
    private:
        Actor *_belong_actor;

    public:
        Characteristic(Actor *actor) { _belong_actor = actor; }

        Actor *BelongActor() { return _belong_actor; }

        std::vector<Characteristic *> &GetCharacteristics();

        Transform *GetTransform();

        /// @brief get the type of the characteristic,
        /// this is used to solve the problem that there is no method like "instanceof" in c++
        /// @return the type of characteristic
        virtual std::string Type() = 0;
    };

    class Transform : public Characteristic
    {
    private:
        FPoint _position;
        float _rotate;
        FVect _scale;

    public:
        Transform(Actor *actor, FPoint position, float rotate, FVect scale)
            : Characteristic(actor), _position(position), _rotate(rotate), _scale(scale) {}

        FPoint Position() { return _position; }
        void Position(FPoint position) { _position = position; }

        float Rotate() { return _rotate; }
        void Rotate(float rotate) { _rotate = rotate; }

        FVect Scale() { return _scale; }
        void Scale(FVect scale) { _scale = scale; }

        std::string Type() { return std::string("Transform"); }
    };

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
        int Render(FPoint *position, float window_pixel_per_unit);

        /// @brief render area is an aabb rect area that encloses the sprite
        FRect RenderArea();

        std::string Type() { return std::string("Renderer"); }

    private:
        FVect DoRotate(FVect vect, float angle);
        FVect DoScale(FVect vect, FVect scale);
    };

    class Camera : public Characteristic
    {
    private:
        float _camera_size = 1.5; // means how many *Unit* from the center of the window to the top.
        FRect _view;
        unsigned int _depth = 0;  // define the render order of cameras
        unsigned int _layers = 0; // which layers the camera can see actors on
    public:
        Camera(Actor *actor) : Characteristic(actor) {}

        float CameraSize() { return _camera_size; }
        void CameraSize(float size) { _camera_size = size; }

        unsigned int Depth() { return _depth; }
        void Depth(unsigned int depth) { _depth = depth; }

        unsigned int Layers() { return _layers; }
        void Layers(unsigned int layers) { _layers = layers; }

        /// @brief view is a rect area that this camera can seen
        FRect View();

        /// @brief detect all visible actor's renderers in camera's view
        /// @param candidate_renderers the renderers to detect
        /// @return the visiable renderers
        std::vector<Renderer *> DetectRenderersInCameraView(std::vector<Actor *> candidate_actors);

        std::string Type() { return std::string("Camera"); }
    };

} // namespace gamo
