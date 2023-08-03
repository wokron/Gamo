#include "gamo_core_py.h"

PYBIND11_MODULE(gamo_core_py, m)
{
    bind_type(m);
    bind_render(m);
    bind_resource(m);
    bind_game(m);
    bind_actor(m);
    bind_input(m);
    bind_physics(m);
}
