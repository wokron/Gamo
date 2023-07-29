#include "pybind11/pybind11.h"
#include "gamo.h"

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gamo;

void bind_type(py::module_ &m);
