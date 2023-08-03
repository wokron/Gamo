#include "gamo_core_py.h"

void bind_resource(py::module_ &m)
{
    py::class_<IResource>(m, "IResource")
        .def("destroy", &IResource::Destroy);
}
