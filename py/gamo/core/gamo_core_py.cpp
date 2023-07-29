#include "gamo_core_py.h"

PYBIND11_MODULE(gamo_core_py, m)
{
    bind_type(m);
    bind_render(m);
}
