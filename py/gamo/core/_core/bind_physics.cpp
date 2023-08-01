#include "gamo_core_py.h"

void bind_physics(py::module_ &m)
{
    py::class_<PhysicsConfig, std::unique_ptr<PhysicsConfig, py::nodelete>>(m, "PhysicsConfig")
        .def(py::init([]()
                      { return PhysicsConfig::GetInstance(); }))
        .def_property("gravity", py::overload_cast<>(&PhysicsConfig::Gravity), py::overload_cast<Vect>(&PhysicsConfig::Gravity))
        .def_property("velocity_iterations", py::overload_cast<>(&PhysicsConfig::VelocityIter), py::overload_cast<int>(&PhysicsConfig::VelocityIter))
        .def_property("position_iterations", py::overload_cast<>(&PhysicsConfig::PositionIter), py::overload_cast<int>(&PhysicsConfig::PositionIter));
        
}