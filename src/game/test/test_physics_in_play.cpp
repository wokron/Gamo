#include "gtest/gtest.h"
#include "director.h"
#include "game.h"
#include "render.h"
#include "actor.h"
#include "SDL2/SDL_image.h"
#include "input.h"

const char *SOURCE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/example.jpeg";

using namespace gamo;

Sprite *GetSprite()
{
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);

    int w, h;
    w = h = 100;
    int ph = texture->PixelHeight(), pw = texture->PixelWidth();

    Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    auto sprite = texture->ClipAndCreateSprite(&cliprect);

    return sprite;
}

class QuitBehavior : public Behavior
{
private:
public:
    QuitBehavior(Actor *actor) : Behavior(actor) {}
    bool start = false;
    void OnStart()
    {
        ASSERT_FALSE(start);
        start = true;
    }
    
    void OnUpdate()
    {
        ASSERT_TRUE(start);

        if (Input::GetInstance()->Quit())
        {
            Play::GetInstance()->Quit(true);
        }
    }
};

TEST(TestPlay, test_physics)
{
    auto play = Play::GetInstance();
    play->Quit(false);
    play->Init("test play test physics", 800, 600, true);
    
    auto sprite = GetSprite();

    auto scene = new Scene();

    auto camera_actor = new Actor({0, 0}, 0, {1, 1});
    auto camera = new Camera(camera_actor);
    camera->Layers(LAYER(0));
    camera_actor->GetCharacteristics().push_back(camera);
    camera_actor->GetCharacteristics().push_back(new QuitBehavior(camera_actor));
    
    auto actor = new Actor({0, 2}, -10, {1, 1});
    actor->Layer(LAYER(0));
    // add renderer
    auto renderer = new Renderer(actor);
    renderer->TargetSprite(sprite);
    actor->GetCharacteristics().push_back(renderer);
    // add rigidbody
    auto rigidbody = new RigidBody(actor);
    rigidbody->Define().type = b2_dynamicBody;
    actor->GetCharacteristics().push_back(rigidbody);
    // add collider
    auto collider = new Collider(actor);
    PolygonShape shape;
    shape.SetAsBox(0.5, 0.5);
    collider->Define().shape = &shape;
    collider->Define().density = 10.0f;
    collider->Define().friction = 0.6f;
    collider->Define().restitution = 0.2f;
    actor->GetCharacteristics().push_back(collider);
    SDL_SetRenderDrawColor(RenderAsset::GetInstance()->Renderer(), 255, 255, 255, 255);

    auto actor2 = new Actor({0.5, 3}, 10, {1, 0.5});
    actor2->Layer(LAYER(0));
    // add renderer
    auto renderer2 = new Renderer(actor2);
    renderer2->TargetSprite(sprite);
    actor2->GetCharacteristics().push_back(renderer2);
    // add rigidbody
    auto rigidbody2 = new RigidBody(actor2);
    rigidbody2->Define().type = b2_dynamicBody;
    actor2->GetCharacteristics().push_back(rigidbody2);
    // add collider
    auto collider2 = new Collider(actor2);
    PolygonShape shape2;
    shape2.SetAsBox(0.5, 0.25);
    collider2->Define().shape = &shape2;
    collider2->Define().density = 20.0f;
    collider2->Define().friction = 0.6f;
    collider2->Define().restitution = 0.2f;
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
    PolygonShape groundshape;
    groundshape.SetAsBox(10, 0.5);
    groundcollider->Define().shape = &groundshape;
    ground_actor->GetCharacteristics().push_back(groundcollider);

    scene->AddActor(actor);
    scene->AddActor(actor2);
    scene->AddActor(ground_actor);
    scene->AddActor(camera_actor);

    play->PushScene(scene);

    play->Perform();

    play->Destroy();
}
