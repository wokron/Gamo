#include "gtest/gtest.h"
#include "director.h"
#include "render.h"
#include "actor.h"
#include "SDL2/SDL_image.h"

const char *SOURCE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/example.jpeg";

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

    Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    auto sprite = texture->ClipAndCreateSprite(&cliprect);

    return sprite;
}

TEST(TestRenderDirector, test_singleton)
{
    auto rd = RenderDirector::GetInstance();
    ASSERT_NE(rd, nullptr);

    auto rd2 = RenderDirector::GetInstance();
    ASSERT_NE(rd2, nullptr);
    ASSERT_EQ(rd, rd2);
}

TEST(TestRenderDirector, test_single_camera)
{
    Init();

    Actor *actors[] = {
        new Actor({0, 0}, 0, {1, 1}),
        new Actor({1, 1}, 0, {1, 1}),
        new Actor({2, 1.5}, 0, {1, 1}),
        new Actor({-1, 1.5}, 0, {1, 1}),
        new Actor({1, 2.5}, 0, {1, 1}),
        new Actor({-1, 2.5}, 0, {1, 1}),
        new Actor({1.5, -0.5}, 0, {1, 1}),
    };

    auto sprite = GetSprite();
    for (int i = 0; i < 7; i++)
    {
        auto renderer = new Renderer(actors[i]);
        renderer->TargetSprite(sprite);
        actors[i]->GetCharacteristics().push_back(renderer);
    }

    actors[0]->Layer(LAYER(1));
    actors[1]->Layer(LAYER(3));
    actors[2]->Layer(LAYER(3));
    actors[3]->Layer(LAYER(2));
    actors[4]->Layer(LAYER(3));
    actors[5]->Layer(LAYER(4));
    actors[6]->Layer(LAYER(5));

    auto camera_actor = new Actor({0, 0}, 0, {0.2, 0.2});
    auto camera = new Camera(camera_actor);
    camera->Layers(LAYER(1) | LAYER(2) | LAYER(3));
    camera_actor->GetCharacteristics().push_back(camera);
    auto renderer = new Renderer(camera_actor);
    renderer->TargetSprite(sprite);
    camera_actor->GetCharacteristics().push_back(renderer);
    camera_actor->Layer(LAYER(1));

    auto rd = RenderDirector::GetInstance();

    std::vector<Actor *> actor_list;
    for (int i = 0; i < 7; i++)
        actor_list.push_back(actors[i]);

    actor_list.push_back(camera_actor);

    rd->RegisterCamera(camera);

    for (float i = 0; i < 3; i += 0.2)
    {
        camera_actor->GetTransform()->Position({i / 2, i});
        rd->DetectRenderer(actor_list);
        rd->Render();

        SDL_Delay(50);
    }

    rd->UnRegisterAllCameras();

    for (float i = 0; i < 3; i += 0.2)
    {
        camera_actor->GetTransform()->Position({i / 2, i});
        rd->DetectRenderer(actor_list);
        rd->Render();

        SDL_Delay(50);
    }

    Destroy();
}

TEST(TestRenderDirector, test_render_level)
{
    Init();

    std::vector<Actor *> actors = {
        new Actor({0, 0}, 0, {1, 1}),
        new Actor({0.5, 0}, 0, {1, 1}),
        new Actor({0.25, 0.5}, 0, {1, 1}),
    };

    auto sprite = GetSprite();

    // red
    auto renderer = new Renderer(actors[0]);
    renderer->TargetSprite(sprite);
    renderer->ColorConfig({255, 0, 0, 255});
    actors[0]->GetCharacteristics().push_back(renderer);

    // green
    renderer = new Renderer(actors[1]);
    renderer->TargetSprite(sprite);
    renderer->ColorConfig({0, 255, 0, 255});
    actors[1]->GetCharacteristics().push_back(renderer);

    // blue
    renderer = new Renderer(actors[2]);
    renderer->TargetSprite(sprite);
    renderer->ColorConfig({0, 0, 255, 255});
    actors[2]->GetCharacteristics().push_back(renderer);

    auto camera_actor = new Actor({0, 0}, 0, {0.2, 0.2});
    auto camera1 = new Camera(camera_actor);
    camera_actor->GetCharacteristics().push_back(camera1);
    auto camera2 = new Camera(camera_actor);
    camera_actor->GetCharacteristics().push_back(camera2);

    auto rd = RenderDirector::GetInstance();
    rd->RegisterCamera(camera1);
    rd->RegisterCamera(camera2);
    
    // all in same layer
    actors[0]->Layer(LAYER(0));
    actors[1]->Layer(LAYER(0));
    actors[2]->Layer(LAYER(0));
    camera1->Layers(LAYER(0));
    camera2->Layers(LAYER(0));
    rd->DetectRenderer(actors);
    rd->Render();
    SDL_Delay(1000);

    // blue in camera 2 and depth2 < depth1; red and green in camera 1 and level_red > level_green, so r-g-b
    actors[0]->Layer(LAYER(1));
    actors[1]->Layer(LAYER(1));
    actors[2]->Layer(LAYER(2));
    camera1->Layers(LAYER(1));
    camera2->Layers(LAYER(2));
    camera2->Depth(0);
    camera1->Depth(10);
    ((Renderer*)actors[0]->GetCharacteristicByType("Renderer"))->RenderLevel(10);
    ((Renderer*)actors[1]->GetCharacteristicByType("Renderer"))->RenderLevel(0);
    rd->DetectRenderer(actors);
    rd->Render();
    SDL_Delay(1000);

    // now depth2 > depth1, so b-r-g
    camera2->Depth(10);
    camera1->Depth(0);
    rd->DetectRenderer(actors);
    rd->Render();
    SDL_Delay(1000);

    // now level_red < level_green, so b-g-r
    ((Renderer*)actors[0]->GetCharacteristicByType("Renderer"))->RenderLevel(0);
    ((Renderer*)actors[1]->GetCharacteristicByType("Renderer"))->RenderLevel(10);
    rd->DetectRenderer(actors);
    rd->Render();
    SDL_Delay(1000);

    // change red to camera 2 and make level_red > level_blue, so r-b-g
    actors[0]->Layer(LAYER(2));
    ((Renderer*)actors[0]->GetCharacteristicByType("Renderer"))->RenderLevel(10);
    ((Renderer*)actors[2]->GetCharacteristicByType("Renderer"))->RenderLevel(0);
    rd->DetectRenderer(actors);
    rd->Render();
    SDL_Delay(1000);

    Destroy();
}

TEST(TestRenderDirector, test_double_cameras)
{
    Init();

    Actor *actors[] = {
        new Actor({0, 0}, 0, {1, 1}),
        new Actor({1, 1}, 0, {1, 1}),
        new Actor({2, 1.5}, 0, {1, 1}),
        new Actor({-1, 1.5}, 0, {1, 1}),
        new Actor({1, 2.5}, 0, {1, 1}),
        new Actor({-1, 2.5}, 0, {1, 1}),
        new Actor({1.5, -0.5}, 0, {1, 1}),
    };

    auto sprite = GetSprite();
    for (int i = 0; i < 5; i++)
    {
        auto renderer = new Renderer(actors[i]);
        renderer->TargetSprite(sprite);
        actors[i]->GetCharacteristics().push_back(renderer);
    }

    for (int i = 5; i < 7; i++)
    {
        auto renderer = new Renderer(actors[i]);
        renderer->TargetSprite(sprite);
        renderer->ColorConfig({255, 255, 0, 255});
        actors[i]->GetCharacteristics().push_back(renderer);
    }

    actors[0]->Layer(LAYER(1));
    actors[1]->Layer(LAYER(3));
    actors[2]->Layer(LAYER(3));
    actors[3]->Layer(LAYER(2));
    actors[4]->Layer(LAYER(3));
    actors[5]->Layer(LAYER(4));
    actors[6]->Layer(LAYER(5));

    auto camera_actor = new Actor({0, 0}, 0, {0.2, 0.2});
    auto camera = new Camera(camera_actor);
    camera->Layers(LAYER(1) | LAYER(2) | LAYER(3));
    camera_actor->GetCharacteristics().push_back(camera);
    auto renderer = new Renderer(camera_actor);
    renderer->TargetSprite(sprite);
    camera_actor->GetCharacteristics().push_back(renderer);

    auto camera_actor2 = new Actor({0, 0}, 0, {0.2, 0.2});
    auto camera2 = new Camera(camera_actor2);
    camera2->Layers(LAYER(3) | LAYER(4) | LAYER(5));
    camera_actor2->GetCharacteristics().push_back(camera);
    auto renderer2 = new Renderer(camera_actor2);
    renderer2->TargetSprite(sprite);
    camera_actor2->GetCharacteristics().push_back(renderer2);

    auto rd = RenderDirector::GetInstance();

    std::vector<Actor *> actor_list;
    for (int i = 0; i < 7; i++)
        actor_list.push_back(actors[i]);

    actor_list.push_back(camera_actor);
    actor_list.push_back(camera_actor2);

    rd->RegisterCamera(camera);
    rd->RegisterCamera(camera2);

    for (float i = 0; i < 3; i += 0.2)
    {
        camera_actor->GetTransform()->Position({i / 2, i});
        camera_actor2->GetTransform()->Position({-i / 2, i});
        rd->DetectRenderer(actor_list);
        rd->Render();

        SDL_Delay(100);
    }

    Destroy();
}
