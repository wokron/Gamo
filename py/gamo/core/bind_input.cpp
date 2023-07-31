#include "gamo_core_py.h"

void bind_input(py::module_ &m)
{
    py::class_<Input, std::unique_ptr<Input, py::nodelete>>(m, "Input")
        .def(py::init([]()
                      { return Input::GetInstance(); }))
        .def_property_readonly("is_quit", &Input::Quit)
        .def("get_key", &Input::GetKey, "scancode"_a)
        .def("get_keydown", &Input::GetKeyDown, "scancode"_a)
        .def("get_keyup", &Input::GetKeyUp, "scancode"_a)
        .def("get_mouse_button", &Input::GetMouseButton, "button_no"_a)
        .def("get_mouse_buttondown", &Input::GetMouseButtonDown, "button_no"_a)
        .def("get_mouse_buttonup", &Input::GetMouseButtonUp, "button_no"_a)
        .def_property_readonly("mouse_position", [](Input &self){ 
            int x, y;
            self.GetMousePosition(&x, &y);
            return Vect{(float)x, (float)y};
        });
}
