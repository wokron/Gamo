#include <math.h>
#include "gtest/gtest.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "render.h"

using namespace gamo;

const char *SOURCE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/example.jpeg";
const char *ANIMATE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/walk_animate.png"; // this image come from Lazy Foo' Productions（http://lazyfoo.net/） 
const int FRAME_DELAY = 50;

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

TEST(TestSprite, sprite_clip)
{
    Init();
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);
    ASSERT_NE(texture, nullptr);

    int w, h;
    w = h = 100;
    int ph = texture->PixelHeight(), pw = texture->PixelWidth();

    SDL_Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    auto sprite = texture->ClipAndCreateSprite(&cliprect);
    ASSERT_NE(sprite, nullptr);

    auto pivot = sprite->Pivot();
    ASSERT_EQ(pivot.x, 0.5);
    ASSERT_EQ(pivot.y, 0.5);

    Destroy();
}

void SpriteRender(Sprite *s, SDL_FPoint *position, float rotate, SDL_FPoint *scale, float window_pixel_per_unit)
{
    SDL_RenderClear(g_renderer);

    ColorAlpha coloralpha = {255, 255, 255, 255};

    s->Render(position, rotate, scale, &coloralpha, window_pixel_per_unit);

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

    SDL_Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    Sprite *s = texture->ClipAndCreateSprite(&cliprect);
    ASSERT_NE(s, nullptr);

    // center
    SDL_FPoint pos1 = {2, 1.5}; // the unit is *Unit*
    SpriteRender(s, &pos1, 0, nullptr, wppu);

    // rotate
    SpriteRender(s, &pos1, 30, nullptr, wppu);

    // scale
    SDL_FPoint scale1 = {2, 1};
    SpriteRender(s, &pos1, 0, &scale1, wppu);

    // rotate and scale
    SpriteRender(s, &pos1, 30, &scale1, wppu);

    // rotate and flip scale
    SDL_FPoint scale2 = {-2, 1};
    SpriteRender(s, &pos1, 30, &scale2, wppu);

    // rotate and flip scale in both horizontal and vertical
    SDL_FPoint scale3 = {-2, -1};
    SpriteRender(s, &pos1, 30, &scale3, wppu);

    // center again
    SpriteRender(s, &pos1, 0, nullptr, wppu);
    
    // change the pivot so the center will change
    SDL_FPoint pivot = {0.3, 0.3};
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

    // change the pivot so the center will change, this time pivot value will be out of [0, 1]
    SDL_FPoint pivot2 = {-0.5, 0.5};
    ss->Pivot(pivot2);
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

TEST(TestSprite, sprite_render_color)
{
    int r;

    Init();

    float camera_size = 1.5; // means window's height is three *Unit* 
    float wppu = 300 / camera_size;
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);
    ASSERT_NE(texture, nullptr);
    texture->PixelPerUnit(100);

    int w, h;
    w = h = 100;
    int ph = texture->PixelHeight(), pw = texture->PixelWidth();

    SDL_Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    Sprite *s = texture->ClipAndCreateSprite(&cliprect);
    ASSERT_NE(s, nullptr);

    SDL_FPoint flip = {1, 1};

    for (int i = 0; i <= 255; i += 50)
    {
        SDL_RenderClear(g_renderer);

        ColorAlpha coloralpha = {255, 255, (u_char)i, 255};
        
        SDL_FPoint pos = {2, 1.5};
        SDL_FPoint scale = {1, 1};
        r = s->Render(&pos, 30, &scale, &coloralpha, wppu);
        ASSERT_EQ(r, 0);

        SDL_RenderPresent(g_renderer);

        SDL_Delay(FRAME_DELAY);
    }

    for (int i = 255; i >= 0; i -= 50)
    {
        SDL_RenderClear(g_renderer);

        ColorAlpha coloralpha = {255, 255, 255, (u_char)i};
        
        SDL_FPoint pos = {2, 1.5};
        SDL_FPoint scale = {1, 1};
        r = s->Render(&pos, 30, &scale, &coloralpha, wppu);
        ASSERT_EQ(r, 0);

        SDL_RenderPresent(g_renderer);

        SDL_Delay(FRAME_DELAY);
    }

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

    SDL_Rect cliprect = {pw / 2 - w / 2, ph / 2 - h / 2, w, h};
    Sprite *s = texture->ClipAndCreateSprite(&cliprect);
    ASSERT_NE(s, nullptr);

    SDL_FPoint pivot = {0.3, 0.7};
    SingleSprite *ss = (SingleSprite *)s;
    ss->Pivot(pivot);

    for (int i = 0; i < 30; i++)
    {
        // x in [0, 4], y in [0, 3]
        SDL_FPoint pos = {2 * (sinf(0.05f * i - 5) + 1), 1.5f * (cosf(0.017f * i) + 1)};
        // angle in [0, 360]
        float rotate = 180 * (sinf(0.03 * i + 3) + 1);
        // scale can be in [-2, 2]
        SDL_FPoint scale = {2 * (cosf(0.07 * i)), 2 * (sinf(0.06 * i + 10))};

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
        SDL_Rect cliprect = {w * i, 0, w, h};
        sprites[i] = texture->ClipAndCreateSprite(&cliprect);
    }

    auto animation = new AnimateSprite();

    for (int i = 0; i < 4; i++)
    {
        animation->AddFrame(1, sprites[i]);
    }

    SDL_FPoint pos = {2, 1.5}; // the unit is *Unit*
    for (int i = 0; i < 30; i++)
    {
        SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
        SpriteRender(animation, &pos, 0, nullptr, wppu);
    }

    Destroy();
}