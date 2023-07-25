#include "gtest/gtest.h"
#include "actor.h"
#include "render.h"
#include <SDL2/SDL_image.h>
#include <algorithm>

using namespace gamo;

const char *SOURCE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/example.jpeg";

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

    SDL_Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    auto sprite = texture->ClipAndCreateSprite(&cliprect);

    return sprite;
}

TEST(TestCharacteristic, test_render_basic)
{
    Init();

    auto sprite = GetSprite();

    auto actor = new Actor({0, 0}, 0, {1, 1});
    auto renderer = new Renderer(actor);
    renderer->TargetSprite(sprite);
    actor->GetCharacteristics().push_back(renderer);
    ASSERT_EQ(actor->GetCharacteristics().size(), 2);

    auto camera = new Camera(actor);
    actor->GetCharacteristics().push_back(camera);
    ASSERT_EQ(actor->GetCharacteristics().size(), 3);

    auto rendercall = [renderer, camera]()
    {
        int win_width, win_height;
        SDL_GetWindowSize(g_window, &win_width, &win_height);

        float h_size = camera->CameraSize();
        float w_size = h_size / win_height * win_width;
        float wppu = win_height / (2 * h_size);

        auto render_pos = renderer->GetTransform()->Position();
        auto camera_pos = camera->GetTransform()->Position();
        // from world coordinate to window coordinate, y axies need to reverse.
        SDL_FPoint pos = {w_size + (render_pos.x - camera_pos.x), h_size - (render_pos.y - camera_pos.y)};
        renderer->Render(&pos, wppu);
    };

    SDL_RenderClear(g_renderer);
    rendercall();
    SDL_RenderPresent(g_renderer);
    SDL_Delay(100);

    actor->GetTransform()->Position({100, 10000});

    SDL_RenderClear(g_renderer);
    rendercall();
    SDL_RenderPresent(g_renderer);
    SDL_Delay(100);

    Destroy();
}

void Render(Camera *camera, Renderer *renderer)
{
    int win_width, win_height;
    SDL_GetWindowSize(g_window, &win_width, &win_height);

    float h_size = camera->CameraSize();
    float w_size = h_size / win_height * win_width;
    float wppu = win_height / (2 * h_size);

    auto render_pos = renderer->GetTransform()->Position();
    auto camera_pos = camera->GetTransform()->Position();
    // from world coordinate to window coordinate, y axies need to reverse.
    SDL_FPoint pos = {w_size + (render_pos.x - camera_pos.x), h_size - (render_pos.y - camera_pos.y)};
    renderer->Render(&pos, wppu);
}

TEST(TestCharacteristic, test_camera)
{
    Init();

    auto sprite = GetSprite();

    auto camera_actor = new Actor({0, 0}, 0, {0.2, 0.2});
    camera_actor->GetCharacteristics().push_back(new Camera(camera_actor));
    camera_actor->GetCharacteristics().push_back(new Renderer(camera_actor));

    auto actor = new Actor({0, 0}, 0, {1.5, 1});
    actor->GetCharacteristics().push_back(new Renderer(actor));

    auto camera_renderer = (Renderer *)camera_actor->GetCharacteristicByType("Renderer");
    auto camera = (Camera *)camera_actor->GetCharacteristicByType("Camera");
    auto renderer = (Renderer *)actor->GetCharacteristicByType("Renderer");
    camera_renderer->TargetSprite(sprite);
    renderer->TargetSprite(sprite);

    auto rendercall = [renderer, camera]()
    {
        int win_width, win_height;
        SDL_GetWindowSize(g_window, &win_width, &win_height);

        float h_size = camera->CameraSize();
        float w_size = h_size / win_height * win_width;
        float wppu = win_height / (2 * h_size);

        auto render_pos = renderer->GetTransform()->Position();
        auto camera_pos = camera->GetTransform()->Position();
        // from world coordinate to window coordinate, y axies need to reverse.
        SDL_FPoint pos = {w_size + (render_pos.x - camera_pos.x), h_size - (render_pos.y - camera_pos.y)};
        renderer->Render(&pos, wppu);
    };

    for (float i = -2; i < 2; i += 0.1)
    {
        camera->GetTransform()->Position({0, i});
        actor->GetTransform()->Position({i, 0});
        SDL_RenderClear(g_renderer);
        Render(camera, renderer);
        Render(camera, camera_renderer);
        SDL_RenderPresent(g_renderer);
        SDL_Delay(50);
    }

    Destroy();
}

TEST(TestCharacteristic, test_render_area)
{
    Init();

    auto sprite = GetSprite();

    auto actor = new Actor({0, 0}, 0, {1, 1});
    actor->GetCharacteristics().push_back(new Renderer(actor));
    auto renderer = (Renderer *)actor->GetCharacteristicByType("Renderer");
    renderer->TargetSprite(sprite);

    auto area = renderer->RenderArea();
    ASSERT_FLOAT_EQ(area.h, 1);
    ASSERT_FLOAT_EQ(area.w, 1);
    ASSERT_FLOAT_EQ(area.x, -0.5);
    ASSERT_FLOAT_EQ(area.y, -0.5);

    actor->GetTransform()->Position({1, 2});
    area = renderer->RenderArea();
    ASSERT_FLOAT_EQ(area.h, 1);
    ASSERT_FLOAT_EQ(area.w, 1);
    ASSERT_FLOAT_EQ(area.x, 0.5);
    ASSERT_FLOAT_EQ(area.y, 1.5);

    actor->GetTransform()->Scale({1, 2});
    area = renderer->RenderArea();
    ASSERT_FLOAT_EQ(area.h, 2);
    ASSERT_FLOAT_EQ(area.w, 1);
    ASSERT_FLOAT_EQ(area.x, 0.5);
    ASSERT_FLOAT_EQ(area.y, 1);

    actor->GetTransform()->Rotate(2);
    area = renderer->RenderArea();
    ASSERT_GT(area.h, 2);
    ASSERT_GT(area.w, 1);

    // again, with different pivot
    auto t = actor->GetTransform();
    t->Position({0, 0});
    t->Rotate(0);
    t->Scale({1, 1});

    ((SingleSprite *)sprite)->Pivot({0, 0.5});

    area = renderer->RenderArea();
    ASSERT_FLOAT_EQ(area.h, 1);
    ASSERT_FLOAT_EQ(area.w, 1);
    ASSERT_FLOAT_EQ(area.x, 0);
    ASSERT_FLOAT_EQ(area.y, -0.5);

    actor->GetTransform()->Position({1, 2});
    area = renderer->RenderArea();
    ASSERT_FLOAT_EQ(area.h, 1);
    ASSERT_FLOAT_EQ(area.w, 1);
    ASSERT_FLOAT_EQ(area.x, 1);
    ASSERT_FLOAT_EQ(area.y, 1.5);

    actor->GetTransform()->Scale({1, 2});
    area = renderer->RenderArea();
    ASSERT_FLOAT_EQ(area.h, 2);
    ASSERT_FLOAT_EQ(area.w, 1);
    ASSERT_FLOAT_EQ(area.x, 1);
    ASSERT_FLOAT_EQ(area.y, 1);

    actor->GetTransform()->Rotate(2);
    area = renderer->RenderArea();
    ASSERT_GT(area.h, 2);
    ASSERT_GT(area.w, 1);

    // now test sub actor's render area
    ((SingleSprite *)sprite)->Pivot({0.5, 0.5});
    auto actor1 = new Actor({0, 0}, 0, {1, 1});
    auto actor2 = actor1->CreateSubActor({1, 1}, 0, {2, 1});
    actor2->GetCharacteristics().push_back(new Renderer(actor2));
    auto renderer2 = (Renderer *)actor2->GetCharacteristicByType("Renderer");
    renderer2->TargetSprite(sprite);

    area = renderer2->RenderArea();
    ASSERT_FLOAT_EQ(area.x, 0);
    ASSERT_FLOAT_EQ(area.y, 0.5);
    ASSERT_FLOAT_EQ(area.h, 1);
    ASSERT_FLOAT_EQ(area.w, 2);

    actor1->GetTransform()->Position({1, 2});
    area = renderer2->RenderArea();
    ASSERT_FLOAT_EQ(area.x, 1);
    ASSERT_FLOAT_EQ(area.y, 2.5);
    ASSERT_FLOAT_EQ(area.h, 1);
    ASSERT_FLOAT_EQ(area.w, 2);

    actor1->GetTransform()->Scale({1, 2});
    area = renderer2->RenderArea();
    ASSERT_FLOAT_EQ(area.x, 1);
    ASSERT_FLOAT_EQ(area.y, 2);
    ASSERT_FLOAT_EQ(area.h, 2);
    ASSERT_FLOAT_EQ(area.w, 2);

    Destroy();
}

TEST(TestCharacteristic, test_camera_area)
{
    Init();

    auto actor = new Actor({0, 0}, 0, {1, 1});
    actor->GetCharacteristics().push_back(new Camera(actor));
    auto camera = (Camera *)actor->GetCharacteristicByType("Camera");

    auto area = camera->View();
    ASSERT_FLOAT_EQ(area.h, 3);
    ASSERT_FLOAT_EQ(area.w, 4);
    ASSERT_FLOAT_EQ(area.x, -2);
    ASSERT_FLOAT_EQ(area.y, -1.5);

    actor->GetTransform()->Position({5, 10});
    area = camera->View();
    ASSERT_FLOAT_EQ(area.h, 3);
    ASSERT_FLOAT_EQ(area.w, 4);
    ASSERT_FLOAT_EQ(area.x, 3);
    ASSERT_FLOAT_EQ(area.y, 8.5);

    float expect_w = 16.0f / 3.0f;
    float expect_x = 5 - 8.0f / 3.0f;
    camera->CameraSize(2);
    area = camera->View();
    ASSERT_FLOAT_EQ(area.h, 4);
    ASSERT_FLOAT_EQ(area.w, expect_w);
    ASSERT_FLOAT_EQ(area.x, expect_x);
    ASSERT_FLOAT_EQ(area.y, 8);

    auto actor1 = new Actor({0, 0}, 0, {1, 1});
    auto actor2 = actor1->CreateSubActor({1, 1}, 0, {2, 1});
    actor2->GetCharacteristics().push_back(new Camera(actor2));
    auto camera2 = (Camera *)actor2->GetCharacteristicByType("Camera");

    area = camera2->View();
    ASSERT_FLOAT_EQ(area.h, 3);
    ASSERT_FLOAT_EQ(area.w, 4);
    ASSERT_FLOAT_EQ(area.x, -1);
    ASSERT_FLOAT_EQ(area.y, -0.5);

    actor1->GetTransform()->Position({-1, 2});
    area = camera2->View();
    ASSERT_FLOAT_EQ(area.h, 3);
    ASSERT_FLOAT_EQ(area.w, 4);
    ASSERT_FLOAT_EQ(area.x, -2);
    ASSERT_FLOAT_EQ(area.y, 1.5);

    Destroy();
}

// TEST(TestCharacteristic, test_detect_renders_in_camera_view)
// {
//     Init();

//     Actor *actors[] = {
//         new Actor({0, 0}, 0, {1, 1}),
//         new Actor({1, 1}, 0, {1, 1}),
//         new Actor({2, 1.5}, 0, {1, 1}),
//         new Actor({-1, 1.5}, 0, {1, 1}),
//         new Actor({1, 2.5}, 0, {1, 1}),
//         new Actor({-1, 2.5}, 0, {1, 1}),
//         new Actor({1.5, -0.5}, 0, {1, 1}),
//     };

//     auto sprite = GetSprite();
//     for (int i = 0; i < 7; i++)
//     {
//         auto renderer = new Renderer(actors[i]);
//         renderer->TargetSprite(sprite);
//         actors[i]->GetCharacteristics().push_back(renderer);
//     }

//     actors[0]->Layer(LAYER(1));
//     actors[1]->Layer(LAYER(3));
//     actors[2]->Layer(LAYER(3));
//     actors[3]->Layer(LAYER(2));
//     actors[4]->Layer(LAYER(3));
//     actors[5]->Layer(LAYER(4));
//     actors[6]->Layer(LAYER(5));

//     auto camera_actor = new Actor({0, 0}, 0, {1, 1});
//     auto camera = new Camera(camera_actor);
//     camera_actor->GetCharacteristics().push_back(camera);

//     auto detect = camera->DetectRenderersInCameraView(std::vector<Actor *>(actors, actors + 7));
//     ASSERT_EQ(detect.size(), 0);

//     camera->Layers(LAYER(1));
//     detect = camera->DetectRenderersInCameraView(std::vector<Actor *>(actors, actors + 7));
//     ASSERT_EQ(detect.size(), 1);
//     ASSERT_NE(std::count(detect.begin(), detect.end(), actors[0]->GetCharacteristicByType("Renderer")), 0);

//     camera->Layers(LAYER(1) | LAYER(2));
//     detect = camera->DetectRenderersInCameraView(std::vector<Actor *>(actors, actors + 7));
//     ASSERT_EQ(detect.size(), 2);
//     ASSERT_NE(std::count(detect.begin(), detect.end(), actors[0]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_NE(std::count(detect.begin(), detect.end(), actors[3]->GetCharacteristicByType("Renderer")), 0);

//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[1]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[2]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[4]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[5]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[6]->GetCharacteristicByType("Renderer")), 0);

//     camera->Layers(LAYER(1) | LAYER(2) | LAYER(3));
//     detect = camera->DetectRenderersInCameraView(std::vector<Actor *>(actors, actors + 7));
//     ASSERT_EQ(detect.size(), 4);
//     ASSERT_NE(std::count(detect.begin(), detect.end(), actors[0]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_NE(std::count(detect.begin(), detect.end(), actors[1]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_NE(std::count(detect.begin(), detect.end(), actors[2]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_NE(std::count(detect.begin(), detect.end(), actors[3]->GetCharacteristicByType("Renderer")), 0);

//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[4]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[5]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[6]->GetCharacteristicByType("Renderer")), 0);

//     camera->GetTransform()->Position({0, 1});
//     detect = camera->DetectRenderersInCameraView(std::vector<Actor *>(actors, actors + 7));
//     ASSERT_NE(std::count(detect.begin(), detect.end(), actors[4]->GetCharacteristicByType("Renderer")), 0);
//     ASSERT_EQ(std::count(detect.begin(), detect.end(), actors[5]->GetCharacteristicByType("Renderer")), 0);

//     Destroy();
// }