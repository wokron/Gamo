#pragma once

#include "characteristic_base.h"
#include "utils.h"
#include <string>

namespace gamo
{
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
