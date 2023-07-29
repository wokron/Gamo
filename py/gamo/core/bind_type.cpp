#include "gamo_core_py.h"
#include "pybind11/operators.h"

void bind_type(py::module_ &m)
{
    py::class_<Vect>(m, "Vect")
        .def(py::init<>())
        .def(py::init([](float x, float y)
                      { return Vect{x, y}; }))
        .def_readwrite("x", &Vect::x)
        .def_readwrite("y", &Vect::y)
        .def(py::self + Vect())
        .def(py::self - Vect())
        .def(-py::self)
        .def(py::self * Vect())
        .def(py::self * float())
        .def(py::self / float())
        .def("__repr__", &Vect::ToString);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>())
        .def(py::init([](float _11, float _12, float _21, float _22)
                      { return Matrix{_11, _12, _21, _22}; }))
        .def_readwrite("_11", &Matrix::_11)
        .def_readwrite("_12", &Matrix::_12)
        .def_readwrite("_21", &Matrix::_21)
        .def_readwrite("_22", &Matrix::_22)
        .def(py::self * Vect())
        .def(py::self * py::self)
        .def("as_rotate", &Matrix::AsRotate)
        .def("as_scale", &Matrix::AsScale)
        .def("__repr__", &Matrix::ToString);

    py::class_<Rect>(m, "Rect")
        .def(py::init<>())
        .def(py::init([](float x, float y, float w, float h)
                      { return Rect{x, y, w, h}; }))
        .def_readwrite("x", &Rect::x)
        .def_readwrite("y", &Rect::y)
        .def_readwrite("w", &Rect::w)
        .def_readwrite("h", &Rect::h)
        .def("vect_xy", &Rect::VectXY)
        .def("vect_wh", &Rect::VectWH)
        .def("__repr__", &Rect::ToString);
}
