#include "gtest/gtest.h"
#include "director.h"
#include "game.h"
#include "render.h"
#include "actor.h"
#include "SDL2/SDL_image.h"
#include "config.h"

const char *SOURCE_PATH =  RESOURCE_DIR "/example.jpeg";

using namespace gamo;

SDL_Renderer *g_renderer;

SDL_Window *g_window;    

void Init()
{
    int r = RenderAsset::GetInstance()->Init("render test", 800, 600, true);
    ASSERT_EQ(r, 0);

    g_window = RenderAsset::GetInstance()->Window();
    g_renderer = RenderAsset::GetInstance()->Renderer();

    ASSERT_NE(g_window, nullptr);
    ASSERT_NE(g_renderer, nullptr);
}

void Destroy()
{
    RenderAsset::GetInstance()->Destroy();
}

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

TEST(TestScene, test_render)
{
    Init();

    std::vector<Actor *> actors = {
        new Actor({0, 0}, 0, {1, 1}),
        new Actor({1, 1}, 0, {1, 1}),
        new Actor({2, 1.5}, 0, {1, 1}),
        new Actor({-1, 1.5}, 0, {1, 1}),
        new Actor({1, 2.5}, 0, {1, 1}),
        new Actor({-1, 2.5}, 0, {1, 1}),
        new Actor({1.5, -0.5}, 0, {1, 1}),

        // for camera
        new Actor({0, 0}, 0, {1, 1}),
        new Actor({0, 0}, 0, {1, 1}),
    };

    auto sprite = GetSprite();

    for (int i = 0; i < 7; i++)
    {
        auto render = new Renderer(actors[i]);
        render->TargetSprite(sprite);
        actors[i]->GetCharacteristics().push_back(render);
        actors[i]->Layer(LAYER(i % 2));
    }

    for (int i = 7; i < 9; i++)
    {
        auto camera = new Camera(actors[i]);
        camera->Layers(LAYER(i-7));
        actors[i]->GetCharacteristics().push_back(camera);
    }

    auto scene = new Scene();
    for (auto actor : actors)
    {
        scene->AddActor(actor);
    }

    scene->RegisterSystemEvents();

    for (float i = -2; i < 2; i += 0.2)
    {
        actors[7]->GetTransform()->Position({0, i});
        actors[8]->GetTransform()->Position({i, 0});
        ASSERT_EQ(scene->RenderStep(), 0);
        SDL_Delay(50);
    }

    scene->UnregisterSystemEvents();

    Destroy();
}

class EventBehavior : public Behavior
{
public:
    EventBehavior(Actor *actor) : Behavior(actor) {}
    bool start = false;
    void OnStart()
    {
        start = true;
    }

    int update = 0;
    void OnUpdate()
    {
        ASSERT_TRUE(start);
        update++;
    }

    int late_update = 0;
    void OnLateUpdate()
    {
        ASSERT_EQ(update - 1, late_update);
        late_update++;
    }
};

TEST(TestScene, test_event_register)
{
    auto actor = new Actor({0, 0}, 0, {1, 1});
    auto b = new EventBehavior(actor);
    actor->GetCharacteristics().push_back(b);

    auto actor2 = new Actor({0, 0}, 0, {1, 1});
    auto b2 = new EventBehavior(actor2);
    actor2->GetCharacteristics().push_back(b2);

    auto scene = new Scene();

    scene->AddActor(actor);
    scene->AddActor(actor2);

    // now scene finish initialization, start to change scene in Play

    // first, register system events is needed
    scene->RegisterSystemEvents();

    EventDispatcher::GetInstance()->Dispatch(EVENT_ON_START, nullptr);

    // on main loop
    ASSERT_TRUE(b->start);
    ASSERT_TRUE(b2->start);
    for (int i = 0; i < 10; i++)
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_UPDATE, nullptr);
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_LATE_UPDATE, nullptr);
        ASSERT_EQ(b->update, i + 1);
        ASSERT_EQ(b2->update, i + 1);
        ASSERT_EQ(b->late_update, i + 1);
        ASSERT_EQ(b2->late_update, i + 1);
    }

    // scene is replaced by other scene, unregister events before free the scene
    scene->UnregisterSystemEvents();

    for (int i = 0; i < 10; i++)
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_UPDATE, nullptr);
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_LATE_UPDATE, nullptr);
        ASSERT_EQ(b->update, 10);
        ASSERT_EQ(b2->update, 10);
        ASSERT_EQ(b->late_update, 10);
        ASSERT_EQ(b2->late_update, 10);
    }
}
