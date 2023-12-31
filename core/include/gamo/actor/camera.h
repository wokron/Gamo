#pragma once

#include "characteristic_base.h"
#include "gamo/utils/utils.h"
#include "gamo/event/event.h"
#include <string>

namespace gamo
{
    class Renderer;
    
    class Camera : public Characteristic
    {
    private:
        float _camera_size = 1.5; // means how many *Unit* from the center of the window to the top.
        Rect _view;
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
        Rect View();

        EVENT_DECLARE(Camera, HandleRender, EVENT_RENDER);

        void RegisterEvents() override { RegisterHandleRender(); Characteristic::RegisterEvents(); }
        void UnregisterEvents() override { UnregisterHandleRender(); Characteristic::UnregisterEvents(); }

        std::string Type() { return std::string("Camera"); }

        Camera *Clone() override;
    };
} // namespace gamo
