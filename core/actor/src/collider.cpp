#include "actor.h"
#include "collider.h"
#include "assert.h"
#include "spdlog/spdlog.h"

namespace gamo
{
    void Collider::CreateAndRepalceFixture(b2Body *rigidbody, Vect offset, float rotate, Vect scale)
    {
        if (_fixture != nullptr)
        {
            rigidbody->DestroyFixture(_fixture);
            _fixture = nullptr;
        }

        if (_shape == nullptr)
        {
            spdlog::error("fail to init collider, shape unset");
            UnregisterHandleInit();
            return;
        }

        // fixture definition for initial
        b2FixtureDef def;
        def.shape = _shape->ToBox2DShape(offset, rotate, scale);
        def.friction = _friction;
        def.restitution = _restitution;
        def.density = _density;
        def.isSensor = _is_sensor;
        def.filter.categoryBits = _category;
        def.filter.maskBits = _collide_with;
        def.userData.pointer = (uintptr_t)this;

        _fixture = rigidbody->CreateFixture(&def);
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
        auto transform = GetTransform();
        if (transform->IsScaleModified())
        {
            // todo: this part should be used to scale the collider, but I haven't thought of a good solution yet
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
        _shape = shape;
        if (_fixture)
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
        if (_fixture)
            _friction = _fixture->GetFriction();
        return _friction;
    }

    void Collider::Friction(float f)
    {
        _friction = f;
        if (_fixture)
            _fixture->SetFriction(_friction);
    }

    float Collider::Restitution()
    {
        if (_fixture)
            _restitution = _fixture->GetRestitution();
        return _restitution;
    }

    void Collider::Restitution(float r)
    {
        _restitution = r;
        if (_fixture)
            _fixture->SetRestitution(_restitution);
    }

    float Collider::Density()
    {
        if (_fixture)
            _density = _fixture->GetDensity();
        return _density;
    }

    void Collider::Density(float d)
    {
        _density = d;
        if (_fixture)
            _fixture->SetDensity(_density);
    }

    bool Collider::IsSensor()
    {
        if (_fixture)
            _is_sensor = _fixture->IsSensor();
        return _is_sensor;
    }

    void Collider::IsSensor(bool is_sensor)
    {
        _is_sensor = is_sensor;
        if (_fixture)
            _fixture->SetSensor(_is_sensor);
    }

    unsigned short Collider::Category()
    {
        if (_fixture)
            _category = _fixture->GetFilterData().categoryBits;
        return _category;
    }

    void Collider::Category(unsigned short c)
    {
        _category = c;
        if (_fixture)
        {
            auto f = _fixture->GetFilterData();
            f.categoryBits = _category;
            _fixture->SetFilterData(f);
        }
    }

    unsigned short Collider::CollideWith()
    {
        if (_fixture)
            _collide_with = _fixture->GetFilterData().maskBits;
        return _collide_with;
    }

    void Collider::CollideWith(unsigned short c)
    {
        _collide_with = c;
        if (_fixture)
        {
            auto f = _fixture->GetFilterData();
            f.maskBits = _collide_with;
            _fixture->SetFilterData(f);
        }
    }

} // namespace gamo
