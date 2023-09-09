#include "gamo/actor/actor.h"
#include "gamo/actor/collider.h"
#include "assert.h"
#include "spdlog/spdlog.h"

namespace gamo
{
    void Collider::CreateAndRepalceFixture(b2Body *rigidbody, Vect offset, float rotate, Vect scale)
    {   
        if (_shape == nullptr)
        {
            spdlog::error("fail to init collider, shape unset");
            return;
        }

        while (!_fixtures.empty())
        {
            auto fixture = _fixtures.back();
            _fixtures.pop_back();
            rigidbody->DestroyFixture(fixture);
        }

        _prev_offset = offset;
        _prev_rotate = rotate;
        _prev_scale = scale;

        for (auto shape : _shape->ToBox2DShape(offset, rotate * M_PI / 180, scale))
        {
            // fixture definition for initial
            b2FixtureDef def;
            def.shape = shape;
            def.friction = _friction;
            def.restitution = _restitution;
            def.density = _density;
            def.isSensor = _is_sensor;
            def.filter.categoryBits = _category;
            def.filter.maskBits = _collide_with;
            def.userData.pointer = (uintptr_t)this;

            auto new_fixture = rigidbody->CreateFixture(&def);
            _fixtures.push_back(new_fixture);
        }
    }

    RigidBody *Collider::SearchRigidBody(Vect *rt_offset, float *rt_rotate, Vect *rt_scale)
    {
        Vect offset = {0, 0};
        float rotate = 0;
        Vect scale = {1, 1};
        auto cur_actor = BelongActor();

        while (cur_actor->SupActor() != nullptr)
        {
            rotate += cur_actor->GetTransform()->Rotate();
            scale = scale * cur_actor->GetTransform()->Scale();
            Matrix rotate_m; rotate_m.AsRotate(cur_actor->SupActor()->GetTransform()->Rotate() * M_PI / 180);
            offset = rotate_m * offset + cur_actor->GetTransform()->Position();
            cur_actor = cur_actor->SupActor();
        }
        scale = scale * cur_actor->GetTransform()->Scale();

        auto rigidbody = (RigidBody *)cur_actor->GetCharacteristicByType("RigidBody"); // rigidbody should be in the root actor
        if (rigidbody == nullptr)
        {
            return nullptr;
        }
        
        if (rt_offset) *rt_offset = offset;
        if (rt_rotate) *rt_rotate = rotate;
        if (rt_scale) *rt_scale = scale;
        return rigidbody;
    }

    void Collider::HandleInit(Event *e)
    {
        Vect offset, scale;
        float rotate;
        auto rigidbody = SearchRigidBody(&offset, &rotate, &scale);
        if (rigidbody != nullptr)
        {
            assert(rigidbody->Body() != nullptr);

            CreateAndRepalceFixture(rigidbody->Body(), offset, rotate, scale);
        }
        UnregisterHandleInit();
    }

    void Collider::HandleBeforeStep(Event *e)
    {
        Vect offset, scale;
        float rotate;
        auto rigidbody = SearchRigidBody(&offset, &rotate, &scale);
        if (rigidbody != nullptr && (offset != _prev_offset || rotate != _prev_rotate || scale != _prev_scale))
        {
            assert(rigidbody->Body() != nullptr);
            
            CreateAndRepalceFixture(rigidbody->Body(), offset, rotate, scale);
        }
    }

    void Collider::DispatchCollisionBegin(std::vector<Collider *> &others)
    {
        auto behaviors = BelongActor()->GetCharacteristicsByType("Behavior");
        while (!others.empty())
        {
            for (auto item : behaviors)
            {
                auto behavior = (Behavior *)item;
                behavior->OnCollisionBegin(others.back());
            }
            others.pop_back();
        }
    }

    void Collider::DispatchCollisionEnd(std::vector<Collider *> &others)
    {
        auto behaviors = BelongActor()->GetCharacteristicsByType("Behavior");
        while (!others.empty())
        {
            for (auto item : behaviors)
            {
                auto behavior = (Behavior *)item;
                behavior->OnCollisionEnd(others.back());
            }
            others.pop_back();
        }
    }

    Shape *Collider::ColliderShape()
    {
        return _shape;
    }

    void Collider::ColliderShape(Shape *shape)
    {
        if (_shape != nullptr)
        {
            delete _shape;
            _shape = nullptr;
        }
        _shape = shape->Clone();
        if (!_fixtures.empty())
        {
            Vect offset, scale;
            float rotate;
            auto rigidbody = SearchRigidBody(&offset, &rotate, &scale);
            if (rigidbody != nullptr)
            {
                assert(rigidbody->Body() != nullptr);

                CreateAndRepalceFixture(rigidbody->Body(), offset, rotate, scale);
            }
        }
    }

    float Collider::Friction()
    {
        return _friction;
    }

    void Collider::Friction(float f)
    {
        _friction = f;
        for (auto fixture : _fixtures)
            fixture->SetFriction(_friction);
    }

    float Collider::Restitution()
    {
        return _restitution;
    }

    void Collider::Restitution(float r)
    {
        _restitution = r;
        for (auto fixture : _fixtures)
            fixture->SetRestitution(_restitution);
    }

    float Collider::Density()
    {
        return _density;
    }

    void Collider::Density(float d)
    {
        _density = d;
        for (auto fixture : _fixtures)
            fixture->SetDensity(_density);
    }

    bool Collider::IsSensor()
    {
        return _is_sensor;
    }

    void Collider::IsSensor(bool is_sensor)
    {
        _is_sensor = is_sensor;
        for (auto fixture : _fixtures)
            fixture->SetSensor(_is_sensor);
    }

    unsigned short Collider::Category()
    {
        return _category;
    }

    void Collider::Category(unsigned short c)
    {
        _category = c;
        for (auto fixture : _fixtures)
        {
            auto f = fixture->GetFilterData();
            f.categoryBits = _category;
            fixture->SetFilterData(f);
        }
    }

    unsigned short Collider::CollideWith()
    {
        return _collide_with;
    }

    void Collider::CollideWith(unsigned short c)
    {
        _collide_with = c;
        for (auto fixture : _fixtures)
        {
            auto f = fixture->GetFilterData();
            f.maskBits = _collide_with;
            fixture->SetFilterData(f);
        }
    }

    Collider *Collider::Clone()
    {
        auto obj = new Collider(nullptr);
        obj->_fixtures = std::vector<b2Fixture *>();
        obj->_shape = (_shape == nullptr ? nullptr : _shape->Clone());
        obj->_friction = _friction;
        obj->_restitution = _restitution;
        obj->_density = _density;
        obj->_is_sensor = _is_sensor;
        obj->_category = _category;
        obj->_collide_with = _collide_with;
        return obj;
    }

} // namespace gamo
