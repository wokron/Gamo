#pragma once

#include "pybind11/pybind11.h"
#include "gamo.h"

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gamo;

void bind_type(py::module_ &m);
void bind_render(py::module_ &m);
void bind_game(py::module_ &m);
void bind_actor(py::module_ &m);
void bind_input(py::module_ &m);
void bind_physics(py::module_ &m);
void bind_resource(py::module_ &m);
