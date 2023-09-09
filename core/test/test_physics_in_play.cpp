#include "gtest/gtest.h"
#include "director.h"
#include "game.h"
#include "render.h"
#include "actor.h"
#include "SDL2/SDL_image.h"
#include "input.h"
#include <set>
#include "config.h"

const char *SOURCE_PATH =  RESOURCE_DIR "/example.jpeg";

using namespace gamo;

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

class MoveBehavior : public Behavior
{
public:
    RigidBody *rigidbody;

    MoveBehavior(Actor *actor) : Behavior(actor) {}

    void OnStart()
    {
        rigidbody = (RigidBody *)BelongActor()->GetCharacteristicByType("RigidBody");
    }

    void OnUpdate()
    {
        Vect lv = {0, 0};
        if (Input::GetInstance()->GetKey(SDL_SCANCODE_W))
        {
            lv = lv + Vect{0, 1};
        }
        if (Input::GetInstance()->GetKey(SDL_SCANCODE_A))
        {
            lv = lv + Vect{-1, 0};
        }
        if (Input::GetInstance()->GetKey(SDL_SCANCODE_S))
        {
            lv = lv + Vect{0, -1};
        }
        if (Input::GetInstance()->GetKey(SDL_SCANCODE_D))
        {
            lv = lv + Vect{1, 0};
        }

        if (lv == Vect{0, 0})
        {
            return;
        }
        
        rigidbody->LinearVelocity(lv);
    }
};

class CollisionBehavior : public Behavior
{
public:
    CollisionBehavior(Actor *actor) : Behavior(actor) {}

    bool triggered_begin = false;
    bool triggered_end = false;
    std::set<Collider *> colliding;

    RigidBody *rigidbody;

    void OnStart()
    {
        rigidbody = (RigidBody *)BelongActor()->GetCharacteristicByType("RigidBody");
    }

    void OnCollisionBegin(Collider *other)
    {
        triggered_begin = true;
        SDL_SetRenderDrawColor(RenderAsset::GetInstance()->Renderer(), 255, 0, 0, 255);
        colliding.insert(other);
        ASSERT_TRUE(colliding.count(other) > 0);
    }

    void OnCollisionEnd(Collider *other)
    {
        triggered_end = true;
        ASSERT_TRUE(colliding.count(other) > 0);
        colliding.erase(other);
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
    actor->GetCharacteristics().push_back(collider);
    SDL_SetRenderDrawColor(RenderAsset::GetInstance()->Renderer(), 255, 255, 255, 255);
    // add collition detect behavior
    auto b = new CollisionBehavior(actor);
    actor->GetCharacteristics().push_back(b);

    auto actor2 = new Actor({0.5, 3}, 10, {1, 0.5});
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
    shape2.SetAsBox(0.5, 0.5, {0, 0});
    collider2->ColliderShape(&shape2);
    collider2->Density(20.0f);
    collider2->Friction(0.6f);
    collider2->Restitution(0.2f);
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
    Terrain groundshape;
    groundshape.SetAsBox(10, 0.5, {0, 0});
    groundcollider->ColliderShape(&groundshape);
    ground_actor->GetCharacteristics().push_back(groundcollider);

    scene->AddActor(actor);
    scene->AddActor(actor2);
    scene->AddActor(ground_actor);
    scene->AddActor(camera_actor);

    play->PushScene(scene);

    play->Perform();

    ASSERT_TRUE(b->triggered_begin);
    ASSERT_TRUE(b->triggered_end);

    play->Destroy();
}

TEST(TestPlay, test_tilemap_in_play)
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
    actor->GetCharacteristics().push_back(collider);
    auto b = new MoveBehavior(actor);
    actor->GetCharacteristics().push_back(b);
    SDL_SetRenderDrawColor(RenderAsset::GetInstance()->Renderer(), 255, 255, 255, 255);

    auto tilemap_actor = new Actor({0, 0}, 0, {1, 1});
    tilemap_actor->Layer(LAYER(0));
    auto tilemap = new Tilemap(tilemap_actor);
    tilemap_actor->AddCharacteristic(tilemap);
    auto tilemap_rigidbody = new RigidBody(tilemap_actor);
    tilemap_actor->AddCharacteristic(tilemap_rigidbody);

    auto tile_actor_prototype = new Actor({0, 0}, 0, {1, 1});
    auto tile = new Tile(tile_actor_prototype);
    tile_actor_prototype->AddCharacteristic(tile);
    auto tile_renderer = new Renderer(tile_actor_prototype);
    tile_renderer->TargetSprite(sprite);
    tile_actor_prototype->AddCharacteristic(tile_renderer);
    auto tile_collider = new Collider(tile_actor_prototype);
    Terrain tileshape;
    tileshape.SetAsBox(0.5, 0.5, {0, 0});
    tile_collider->ColliderShape(&tileshape);
    tile_actor_prototype->AddCharacteristic(tile_collider);

    for (int i = -3; i <= 3; i++)
    {
        auto actual_tile = tilemap_actor->CreateSubActor(tile_actor_prototype);
        actual_tile->Layer(LAYER(0));
        auto pos_tile = (Tile *)actual_tile->GetCharacteristicByType("Tile");
        pos_tile->Position(i, -2);
    }

    auto actual_tile1 = tilemap_actor->CreateSubActor(tile_actor_prototype);
    actual_tile1->Layer(LAYER(0));
    auto pos_tile = (Tile *)actual_tile1->GetCharacteristicByType("Tile");
    pos_tile->Position(-1, -1);

    auto actual_tile2 = tilemap_actor->CreateSubActor(tile_actor_prototype);
    actual_tile2->Layer(LAYER(0));
    pos_tile = (Tile *)actual_tile2->GetCharacteristicByType("Tile");
    pos_tile->Position(1, -1);

    scene->AddActor(actor);
    scene->AddActor(tilemap_actor);
    scene->AddActor(camera_actor);

    play->PushScene(scene);

    play->Perform();

    play->Destroy();
}