#include "gamo_core_py.h"
#include "pybind11/stl.h"

void bind_actor(py::module_ &m)
{
    py::class_<Actor>(m, "Actor")
        .def(py::init([](){ return new Actor({0, 0}, 0, {1, 1}); }))
        .def(py::init<Vect, float, Vect>(), "position"_a, "rotate"_a, "scale"_a)
        .def_property_readonly("characteristics", &Actor::GetCharacteristics)
        .def_property_readonly("transform", &Actor::GetTransform)
        .def_property_readonly("sub_actors", &Actor::SubActors)
        .def_property_readonly("sup_actor", &Actor::SupActor)
        .def_property("layer", py::overload_cast<>(&Actor::Layer), py::overload_cast<unsigned int>(&Actor::Layer))
        .def("get_characteristic", &Actor::GetCharacteristicByType, "type"_a)
        .def("get_characteristics", &Actor::GetCharacteristicsByType, "type"_a)
        .def("create_sub_actor", &Actor::CreateSubActor, "position"_a, "rotate"_a, "scale"_a);

    py::class_<Characteristic>(m, "Characteristic")
        .def_property_readonly("actor", &Characteristic::BelongActor);

    py::class_<Transform, Characteristic>(m, "Transform")
        .def_property("position", py::overload_cast<>(&Transform::Position), py::overload_cast<Vect>(&Transform::Position))
        .def_property("rotate", py::overload_cast<>(&Transform::Rotate), py::overload_cast<float>(&Transform::Rotate))
        .def_property("scale", py::overload_cast<>(&Transform::Scale), py::overload_cast<Vect>(&Transform::Scale))
        .def_property_readonly("global_position", &Transform::GlobalPosition)
        .def_property_readonly("global_rotate", &Transform::GlobalRotate)
        .def_property_readonly("global_scale", &Transform::GlobalScale)
        .def("__repr__", &Transform::ToString);

    py::class_<Renderer, Characteristic>(m, "Renderer")
        .def(py::init<Actor*>())
        .def_property("sprite", py::overload_cast<>(&Renderer::TargetSprite), py::overload_cast<Sprite*>(&Renderer::TargetSprite))
        .def_property("color", py::overload_cast<>(&Renderer::ColorConfig), py::overload_cast<ColorAlpha>(&Renderer::ColorConfig))
        .def_property("render_level", py::overload_cast<>(&Renderer::RenderLevel), py::overload_cast<unsigned int>(&Renderer::RenderLevel))
        .def_property("visiable", py::overload_cast<>(&Renderer::Visiable), py::overload_cast<bool>(&Renderer::Visiable));

    py::class_<Camera, Characteristic>(m, "Camera")
        .def(py::init<Actor*>())
        .def_property("size", py::overload_cast<>(&Camera::CameraSize), py::overload_cast<float>(&Camera::CameraSize))
        .def_property("depth", py::overload_cast<>(&Camera::Depth), py::overload_cast<unsigned int>(&Camera::Depth))
        .def_property("layers", py::overload_cast<>(&Camera::Layers), py::overload_cast<unsigned int>(&Camera::Layers));

    py::class_<Behavior, Characteristic>(m, "Behavior")
        .def(py::init<Actor*>())
        .def("on_start", &Behavior::OnStart)
        .def("on_update", &Behavior::OnUpdate)
        .def("on_late_update", &Behavior::OnLateUpdate)
        .def("on_destroy", &Behavior::OnDestroy);

    py::class_<RigidBody, Characteristic>(m, "RigidBody")
        .def(py::init<Actor*>())
        .def_property("linear_velocity", py::overload_cast<>(&RigidBody::LinearVelocity), py::overload_cast<Vect>(&RigidBody::LinearVelocity))
        .def_property("angular_velocity", py::overload_cast<>(&RigidBody::AngularVelocity), py::overload_cast<float>(&RigidBody::AngularVelocity))
        .def_property("body_type", py::overload_cast<>(&RigidBody::BodyType), py::overload_cast<b2BodyType>(&RigidBody::BodyType))
        .def_property("linear_damping", py::overload_cast<>(&RigidBody::LinearDamping), py::overload_cast<float>(&RigidBody::LinearDamping))
        .def_property("angular_damping", py::overload_cast<>(&RigidBody::AngularDamping), py::overload_cast<float>(&RigidBody::AngularDamping))
        .def_property("allow_sleep", py::overload_cast<>(&RigidBody::AllowSleep), py::overload_cast<bool>(&RigidBody::AllowSleep))
        .def_property("fixed_rotate", py::overload_cast<>(&RigidBody::FixedRotate), py::overload_cast<bool>(&RigidBody::FixedRotate))
        .def_property("gravity_scale", py::overload_cast<>(&RigidBody::GravityScale), py::overload_cast<float>(&RigidBody::GravityScale))
        .def_property("is_bullet", py::overload_cast<>(&RigidBody::Bullet), py::overload_cast<bool>(&RigidBody::Bullet));

    py::class_<Collider, Characteristic>(m, "Collider")
        .def(py::init<Actor*>())
        .def_property("shape", py::overload_cast<>(&Collider::Shape), py::overload_cast<b2Shape*>(&Collider::Shape))
        .def_property("friction", py::overload_cast<>(&Collider::Friction), py::overload_cast<float>(&Collider::Friction))
        .def_property("restitution", py::overload_cast<>(&Collider::Restitution), py::overload_cast<float>(&Collider::Restitution))
        .def_property("density", py::overload_cast<>(&Collider::Density), py::overload_cast<float>(&Collider::Density))
        .def_property("is_sensor", py::overload_cast<>(&Collider::IsSensor), py::overload_cast<bool>(&Collider::IsSensor))
        .def_property("category", py::overload_cast<>(&Collider::Category), py::overload_cast<unsigned short>(&Collider::Category))
        .def_property("collide_with", py::overload_cast<>(&Collider::CollideWith), py::overload_cast<unsigned short>(&Collider::CollideWith));

}
