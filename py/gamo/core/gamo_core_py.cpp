#include "pybind11/pybind11.h"
#include "utils.h"

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gamo;

PYBIND11_MODULE(core_example, m)
{
    py::class_<Vect>(m, "Vect")
        .def(py::init<>())
        .def_readwrite("x", &Vect::x)
        .def_readwrite("y", &Vect::y);
}
