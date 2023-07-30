#include "gamo_core_py.h"

void bind_game(py::module_ &m)
{
    py::class_<Play, std::unique_ptr<Play, py::nodelete>>(m, "Play")
        .def(py::init([](){ return Play::GetInstance(); }))
        .def_property("target_frame_rate", py::overload_cast<>(&Play::TargetFrameRate), py::overload_cast<int>(&Play::TargetFrameRate))
        .def_property("is_quit", py::overload_cast<>(&Play::Quit), py::overload_cast<bool>(&Play::Quit))
        .def("init", &Play::Init, "title"_a, "w"_a, "h"_a, "accelerate"_a)
        .def("perform", &Play::Perform)
        .def("destroy", &Play::Destroy)
        .def("replace_scene", &Play::ReplaceScene, "scene"_a)
        .def("push_scene", &Play::PushScene, "scene"_a)
        .def("pop_scene", &Play::PopScene);

    py::class_<Scene>(m, "Scene")
        .def(py::init<>())
        .def("add_actor", &Scene::AddActor, "actor"_a);
}
