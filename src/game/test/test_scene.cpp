#include "gtest/gtest.h"
#include "director.h"
#include "game.h"
#include "render.h"
#include "actor.h"
#include "SDL2/SDL_image.h"

const char *SOURCE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/example.jpeg";

using namespace gamo;

void Init()
{
    int r;
    r = SDL_Init(SDL_INIT_EVERYTHING);
    ASSERT_EQ(r, 0);

    r = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP);
    ASSERT_EQ(r, IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP);

    g_window = SDL_CreateWindow(
        "render test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN);

    ASSERT_NE(g_window, nullptr);

    g_renderer = SDL_CreateRenderer(g_window, -1, 0);
    ASSERT_NE(g_renderer, nullptr);
}

void Destroy()
{
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_renderer = nullptr;
    g_window = nullptr;
}

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

    scene->RegisterCameras();

    for (float i = -2; i < 2; i += 0.2)
    {
        actors[7]->GetTransform()->Position({0, i});
        actors[8]->GetTransform()->Position({i, 0});
        ASSERT_EQ(scene->RenderStep(), 0);
        SDL_Delay(50);
    }

    Destroy();
}
