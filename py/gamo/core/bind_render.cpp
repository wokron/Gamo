#include "gamo_core_py.h"

void bind_render(py::module_ &m)
{
    py::class_<Texture>(m, "Texture")
        .def_static("load_texture", &Texture::LoadTexture, "texture_path"_a, "color_key"_a)
        .def_property("pixel_per_unit", py::overload_cast<>(&Texture::PixelPerUnit), py::overload_cast<float>(&Texture::PixelPerUnit))
        .def_property_readonly("pixel_width", &Texture::PixelWidth)
        .def_property_readonly("pixel_height", &Texture::PixelHeight)
        .def("clip", &Texture::ClipAndCreateSprite, "cliprect"_a);

    py::class_<Sprite>(m, "Sprite");

    py::class_<SingleSprite, Sprite>(m, "SingleSprite")
        .def_property("sprite_clip", py::overload_cast<>(&SingleSprite::SpriteClip), py::overload_cast<Rect>(&SingleSprite::SpriteClip))
        .def_property("pivot", py::overload_cast<>(&SingleSprite::Pivot), py::overload_cast<Vect>(&SingleSprite::Pivot))
        .def_property_readonly("unit_width", &SingleSprite::UnitWidth)
        .def_property_readonly("unit_height", &SingleSprite::UnitHeight);

    py::class_<AnimateSprite, Sprite>(m, "AnimateSprite")
        .def(py::init<>())
        .def_property_readonly("pivot", &AnimateSprite::Pivot)
        .def_property_readonly("unit_width", &AnimateSprite::UnitWidth)
        .def_property_readonly("unit_height", &AnimateSprite::UnitHeight)
        .def("add_frame", py::overload_cast<SingleSprite *>(&AnimateSprite::AddFrame), "sprite"_a)
        .def("add_frame", py::overload_cast<uint, SingleSprite *>(&AnimateSprite::AddFrame), "continue_frames"_a, "sprite"_a);

}
