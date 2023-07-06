#include <math.h>
#include "gtest/gtest.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "render.h"

using namespace gamo;

const char *SOURCE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/example.jpeg";
const char *ANIMATE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/walk_animate.png";
const int FRAME_DELAY = 50;

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
        SDL_WINDOW_SHOWN
    );

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

TEST(TestSprite, sprite_clip)
{
    Init();
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);
    ASSERT_NE(texture, nullptr);

    int w, h;
    w = h = 100;
    int ph = texture->PixelHeight(), pw = texture->PixelWidth();

    Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    auto sprite = texture->ClipAndCreateSprite(&cliprect);
    ASSERT_NE(sprite, nullptr);

    auto pivot = sprite->Pivot();
    ASSERT_EQ(pivot.x, w / 2);
    ASSERT_EQ(pivot.y, h / 2);

    Destroy();
}

void SpriteRender(Sprite *s, FPoint *position, float rotate, FPoint *scale, float window_pixel_per_unit)
{
    SDL_RenderClear(g_renderer);

    s->Render(position, rotate, scale, window_pixel_per_unit);

    SDL_RenderPresent(g_renderer);

    SDL_Delay(FRAME_DELAY);
}

TEST(TestSprite, sprite_render)
{
    Init();

    float camera_size = 1.5; // means window's height is three *Unit* 
    float wppu = 300 / camera_size;
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);
    ASSERT_NE(texture, nullptr);
    texture->PixelPerUnit(100);

    int w, h;
    w = h = 100;
    int ph = texture->PixelHeight(), pw = texture->PixelWidth();

    Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    Sprite *s = texture->ClipAndCreateSprite(&cliprect);
    ASSERT_NE(s, nullptr);

    // center
    FPoint pos1 = {2, 1.5}; // the unit is *Unit*
    SpriteRender(s, &pos1, 0, nullptr, wppu);

    // rotate
    SpriteRender(s, &pos1, 30, nullptr, wppu);

    // scale
    FVect scale1 = {2, 1};
    SpriteRender(s, &pos1, 0, &scale1, wppu);

    // rotate and scale
    SpriteRender(s, &pos1, 30, &scale1, wppu);

    // rotate and flip scale
    FVect scale2 = {-2, 1};
    SpriteRender(s, &pos1, 30, &scale2, wppu);

    // rotate and flip scale in both horizontal and vertical
    FVect scale3 = {-2, -1};
    SpriteRender(s, &pos1, 30, &scale3, wppu);

    // center again
    SpriteRender(s, &pos1, 0, nullptr, wppu);
    
    // change the pivot so the center will change
    FPoint pivot = {20, 20};
    SingleSprite *ss = (SingleSprite *)s;
    ss->Pivot(pivot);
    SpriteRender(s, &pos1, 0, nullptr, wppu);

    // rotate again
    SpriteRender(s, &pos1, 30, nullptr, wppu);

    // scale again
    SpriteRender(s, &pos1, 0, &scale1, wppu);

    // rotate and scale again
    SpriteRender(s, &pos1, 30, &scale1, wppu);

    // rotate and flip scale again
    SpriteRender(s, &pos1, 30, &scale2, wppu);

    // rotate and flip scale in both horizontal and vertical again
    SpriteRender(s, &pos1, 30, &scale3, wppu);

    Destroy();
}

TEST(TestSprite, sprite_render_strong)
{
    Init();

    float camera_size = 1.5; // means window's height is three *Unit* 
    float wppu = 300 / camera_size;
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);
    ASSERT_NE(texture, nullptr);
    texture->PixelPerUnit(100);

    int w, h;
    w = h = 100;
    int ph = texture->PixelHeight(), pw = texture->PixelWidth();

    Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    Sprite *s = texture->ClipAndCreateSprite(&cliprect);
    ASSERT_NE(s, nullptr);

    FPoint pivot = {30, 70};
    SingleSprite *ss = (SingleSprite *)s;
    ss->Pivot(pivot);

    for (int i = 0; i < 30; i++)
    {
        // x in [0, 4], y in [0, 3]
        FPoint pos = {2 * (sinf(0.05f * i - 5) + 1), 1.5f * (cosf(0.017f * i) + 1)};
        // angle in [0, 360]
        float rotate = 180 * (sinf(0.03 * i + 3) + 1);
        // scale can be in [-2, 2]
        FVect scale = {2 * (cosf(0.07 * i)), 2 * (sinf(0.06 * i + 10))};

        SpriteRender(s, &pos, rotate, &scale, wppu);
    }

    Destroy();
}

TEST(TestSprite, render_animation)
{
    Init();

    float camera_size = 1.5; // means window's height is three *Unit* 
    float wppu = 300 / camera_size;
    
    Color colorkey = {0, 0xff, 0xff};
    auto texture = Texture::LoadTexture(ANIMATE_PATH, &colorkey);
    ASSERT_NE(texture, nullptr);
    texture->PixelPerUnit(100);

    SingleSprite *sprites[4];

    int th = texture->PixelHeight(), tw = texture->PixelWidth();
    int w = tw / 4, h = th;

    for (int i = 0; i < 4; i++)
    {
        Rect cliprect = {w * i, 0, w, h};
        sprites[i] = texture->ClipAndCreateSprite(&cliprect);
    }

    auto animation = new AnimateSprite();

    for (int i = 0; i < 4; i++)
    {
        animation->AddFrame(1, sprites[i]);
    }

    FPoint pos = {2, 1.5}; // the unit is *Unit*
    for (int i = 0; i < 30; i++)
    {
        SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
        SpriteRender(animation, &pos, 0, nullptr, wppu);
    }

    Destroy();
}