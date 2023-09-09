#include "gtest/gtest.h"
#include "gamo/actor/actor.h"

using namespace gamo;

TEST(TestClone, test_characteristics)
{
    auto actor = new Actor({0, 0}, 0, {1, 1});
    auto b = new Behavior(actor);
    auto bc = b->Clone();
    ASSERT_NE(b, bc);
    ASSERT_EQ(b->BelongActor(), actor);
    ASSERT_EQ(bc->BelongActor(), nullptr);

    auto c = new Camera(actor);
    auto cc = c->Clone();
    ASSERT_NE(c, cc);
    ASSERT_EQ(c->BelongActor(), actor);
    ASSERT_EQ(cc->BelongActor(), nullptr);
    ASSERT_FLOAT_EQ(c->CameraSize(), cc->CameraSize());
    
    auto colide = new Collider(actor);
    auto colidec = colide->Clone();
    ASSERT_NE(colide, colidec);
    ASSERT_EQ(colide->BelongActor(), actor);
    ASSERT_EQ(colidec->BelongActor(), nullptr);
}

TEST(TestClone, test_actor)
{
    auto actor = new Actor({0, 0}, 0, {1, 1});
    auto sub1 = actor->CreateSubActor({1, 2}, 3, {4, 5});
    auto sub2 = sub1->CreateSubActor({5, 4}, 3, {2, 1});
    auto sub3 = actor->CreateSubActor({1, 1}, 1, {1, 1});

    auto actor_clone = actor->Clone();
    ASSERT_NE(actor, actor_clone);
    auto sub_actors = actor_clone->SubActors();
    ASSERT_EQ(sub_actors.size(), 2);
    Actor *sub1c = sub_actors[0], *sub3c = sub_actors[1];
    ASSERT_NE(sub1, sub1c);
    ASSERT_NE(sub3, sub3c);
    ASSERT_EQ(sub1->GetTransform()->Position().x, sub1c->GetTransform()->Position().x);
    ASSERT_EQ(sub1->GetTransform()->Position().y, sub1c->GetTransform()->Position().y);
    ASSERT_EQ(sub1->GetTransform()->Rotate(), sub1c->GetTransform()->Rotate());
    ASSERT_EQ(sub1->GetTransform()->Scale().x, sub1c->GetTransform()->Scale().x);
    ASSERT_EQ(sub1->GetTransform()->Scale().y, sub1c->GetTransform()->Scale().y);
    ASSERT_EQ(sub3->GetTransform()->Position().x, sub3c->GetTransform()->Position().x);
    ASSERT_EQ(sub3->GetTransform()->Position().y, sub3c->GetTransform()->Position().y);
    ASSERT_EQ(sub3->GetTransform()->Rotate(), sub3c->GetTransform()->Rotate());
    ASSERT_EQ(sub3->GetTransform()->Scale().x, sub3c->GetTransform()->Scale().x);
    ASSERT_EQ(sub3->GetTransform()->Scale().y, sub3c->GetTransform()->Scale().y);

    sub_actors = sub1c->SubActors();
    ASSERT_EQ(sub_actors.size(), 1);
    Actor *sub2c = sub_actors[0];
    ASSERT_NE(sub2, sub2c);
    ASSERT_EQ(sub2->GetTransform()->Position().x, sub2c->GetTransform()->Position().x);
    ASSERT_EQ(sub2->GetTransform()->Position().y, sub2c->GetTransform()->Position().y);
    ASSERT_EQ(sub2->GetTransform()->Rotate(), sub2c->GetTransform()->Rotate());
    ASSERT_EQ(sub2->GetTransform()->Scale().x, sub2c->GetTransform()->Scale().x);
    ASSERT_EQ(sub2->GetTransform()->Scale().y, sub2c->GetTransform()->Scale().y);
}

Actor *CreateComplexActor()
{
    auto sprite = (SingleSprite *)0x123456; // mock sprite

    auto actor = new Actor({0, 2}, 30, {2, 1});
    actor->Layer(LAYER(0));
    // add renderer
    auto renderer = new Renderer(actor);
    renderer->TargetSprite(sprite);
    actor->GetCharacteristics().push_back(renderer);
    // add rigidbody
    auto rigidbody = new RigidBody(actor);
    rigidbody->BodyType(b2_dynamicBody);
    actor->GetCharacteristics().push_back(rigidbody);
    // add collider
    auto collider = new Collider(actor);
    Polygon shape;
    shape.SetAsBox(0.5, 0.5, {0, 0});
    collider->ColliderShape(&shape);
    collider->Density(10.0f);
    collider->Friction(0.6f);
    collider->Restitution(0.2f);
    collider->Category(COLLISION_CATEGORY(1));
    collider->CollideWith(COLLISION_CATEGORY(1) | COLLISION_CATEGORY(2) | COLLISION_CATEGORY(3));
    actor->GetCharacteristics().push_back(collider);

    auto actor_sub = actor->CreateSubActor({-1, -0.5}, 10, {0.5, 0.5});
    actor_sub->Layer(LAYER(0));
    // add renderer
    auto renderer_sub = new Renderer(actor_sub);
    renderer_sub->TargetSprite(sprite);
    actor_sub->GetCharacteristics().push_back(renderer_sub);
    // add collider
    auto collider_sub = new Collider(actor_sub);
    Polygon shape_sub;
    shape_sub.SetAsBox(0.5, 0.5, {0, 0});
    collider_sub->ColliderShape(&shape_sub);
    collider_sub->Density(10.0f);
    collider_sub->Friction(0.6f);
    collider_sub->Restitution(0.2f);
    collider_sub->Category(COLLISION_CATEGORY(1));
    collider_sub->CollideWith(COLLISION_CATEGORY(1) | COLLISION_CATEGORY(2) | COLLISION_CATEGORY(3));
    actor_sub->GetCharacteristics().push_back(collider_sub);

    return actor;
}

TEST(TestClone, test_actor_with_characteristic)
{
    auto actor = CreateComplexActor();
    auto actor_clone = actor->Clone();

    ASSERT_NE(actor, actor_clone);
    auto charas = actor_clone->GetCharacteristics();
    ASSERT_EQ(charas.size(), 4);

    auto renderer = (Renderer *)actor->GetCharacteristicByType("Renderer");
    auto rendererc = (Renderer *)actor_clone->GetCharacteristicByType("Renderer");
    ASSERT_NE(renderer, rendererc);
    ASSERT_EQ(renderer->TargetSprite(), rendererc->TargetSprite());
    ASSERT_EQ(renderer->BelongActor(), actor);
    ASSERT_EQ(rendererc->BelongActor(), actor_clone);

    auto collider = (Collider *)actor->GetCharacteristicByType("Collider");
    auto colliderc = (Collider *)actor_clone->GetCharacteristicByType("Collider");
    ASSERT_NE(collider, colliderc);
    ASSERT_NE(collider->ColliderShape(), colliderc->ColliderShape());
    ASSERT_EQ(collider->BelongActor(), actor);
    ASSERT_EQ(colliderc->BelongActor(), actor_clone);
}
