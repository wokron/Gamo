#include "gtest/gtest.h"
#include "collider.h"
#include "game.h"
#include "actor.h"
#include "director.h"
#include "config.h"

using namespace gamo;

const char *SOURCE_PATH =  RESOURCE_DIR "/example.jpeg";

Sprite *GetSprite()
{
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);

    int w, h;
    w = h = 100;
    int ph = texture->PixelHeight(), pw = texture->PixelWidth();

    Rect cliprect = IRect{pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    auto sprite = texture->ClipAndCreateSprite(&cliprect);

    return sprite;
}

TEST(TestPhysics, test_simple)
{
    Play::GetInstance()->Init("test physics", 800, 600, true);

    auto sprite = GetSprite();

    auto actor = new Actor({0, 2}, 30, {1, 1});
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

    auto actor2 = new Actor({0.5, 3}, -10, {0.5, 0.5});
    actor2->Layer(LAYER(0));
    // add renderer
    auto renderer2 = new Renderer(actor2);
    renderer2->TargetSprite(sprite);
    actor2->GetCharacteristics().push_back(renderer2);
    // add rigidbody
    auto rigidbody2 = new RigidBody(actor2);
    rigidbody2->BodyType(b2_dynamicBody);
    actor2->GetCharacteristics().push_back(rigidbody2);
    // add collider
    auto collider2 = new Collider(actor2);
    Polygon shape2;
    shape2.SetAsBox(0.25, 0.25, {0, 0});
    collider2->ColliderShape(&shape2);
    collider2->Density(20.0f);
    collider2->Friction(0.6f);
    collider2->Restitution(0.2f);
    collider2->Category(COLLISION_CATEGORY(2));
    collider2->CollideWith(COLLISION_CATEGORY(1) | COLLISION_CATEGORY(2));
    actor2->GetCharacteristics().push_back(collider2);

    auto ground_actor = new Actor({0, -1}, 0, {20, 1});
    ground_actor->Layer(LAYER(0));
    // add renderer
    auto groundrenderer = new Renderer(ground_actor);
    groundrenderer->TargetSprite(sprite);
    ground_actor->GetCharacteristics().push_back(groundrenderer);
    // add rigidbody
    auto groundbody = new RigidBody(ground_actor);
    ground_actor->GetCharacteristics().push_back(groundbody);
    // add collider
    auto groundcollider = new Collider(ground_actor);
    Polygon groundshape;
    groundshape.SetAsBox(10, 0.5, {0, 0});
    groundcollider->ColliderShape(&groundshape);
    groundcollider->Category(COLLISION_CATEGORY(3));
    ground_actor->GetCharacteristics().push_back(groundcollider);

    auto camera_actor = new Actor({0, 0}, 0, {1, 1});
    camera_actor->Layer(LAYER(0));
    // add camera
    auto camera = new Camera(camera_actor);
    camera->Layers(LAYER(0));
    camera_actor->GetCharacteristics().push_back(camera);

    // register events
    groundrenderer->RegisterHandleCameraDetect();
    renderer->RegisterHandleCameraDetect();
    rigidbody->RegisterHandleAfterStep();
    rigidbody->RegisterHandleBeforeStep();
    rigidbody->RegisterHandleInit();
    collider->RegisterHandleInit();
    collider->RegisterHandleBeforeStep();
    renderer2->RegisterHandleCameraDetect();
    rigidbody2->RegisterHandleAfterStep();
    rigidbody2->RegisterHandleBeforeStep();
    rigidbody2->RegisterHandleInit();
    collider2->RegisterHandleInit();
    collider2->RegisterHandleBeforeStep();
    groundbody->RegisterHandleAfterStep();
    groundbody->RegisterHandleBeforeStep();
    groundbody->RegisterHandleInit();
    groundcollider->RegisterHandleInit();
    groundcollider->RegisterHandleBeforeStep();
    camera->RegisterHandleRender();

    b2World world(PhysicsConfig::GetInstance()->Gravity());

    RigidBodyEvent e(&world);

    const float frame = 60;
    
    for (int i = 0; i < 200; i++)
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_RIGIDBODY_INIT, &e);
        EventDispatcher::GetInstance()->Dispatch(EVENT_COLLIDER_INIT, nullptr);
        EventDispatcher::GetInstance()->Dispatch(EVENT_PHYSICS_BEFORE_STEP, nullptr);
        world.Step(1.0f / frame, 6, 2);
        EventDispatcher::GetInstance()->Dispatch(EVENT_PHYSICS_AFTER_STEP, nullptr);
        EventDispatcher::GetInstance()->Dispatch(EVENT_RENDER, nullptr);
        RenderDirector::GetInstance()->Render();
        SDL_Delay(1000.0f / frame);
    }
    
    // unregister events
    renderer->UnregisterHandleCameraDetect();
    groundrenderer->UnregisterHandleCameraDetect();
    rigidbody->UnregisterHandleAfterStep();
    rigidbody->UnregisterHandleBeforeStep();
    rigidbody->UnregisterHandleInit();
    collider->UnregisterHandleInit();
    collider->UnregisterHandleBeforeStep();
    renderer2->UnregisterHandleCameraDetect();
    rigidbody2->UnregisterHandleAfterStep();
    rigidbody2->UnregisterHandleBeforeStep();
    rigidbody2->UnregisterHandleInit();
    collider2->UnregisterHandleInit();
    collider2->UnregisterHandleBeforeStep();
    groundbody->UnregisterHandleAfterStep();
    groundbody->UnregisterHandleBeforeStep();
    groundbody->UnregisterHandleInit();
    groundcollider->UnregisterHandleInit();
    groundcollider->UnregisterHandleBeforeStep();
    camera->UnregisterHandleRender();

    Play::GetInstance()->Destroy();
}
