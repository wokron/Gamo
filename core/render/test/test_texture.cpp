#include "gtest/gtest.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "render.h"

using namespace gamo;

const char *SOURCE_PATH = "/home/wokron/Code/Projects/Gamo/src/render/test/example.jpeg";
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

TEST(TestTexture, init_and_destroy)
{
    Init();

    Destroy();
}

TEST(TestTexture, texture_loading)
{
    Init();

    auto texture = Texture::LoadTexture("path/not/exist", nullptr);

    ASSERT_EQ(texture, nullptr);

    texture = Texture::LoadTexture(SOURCE_PATH, nullptr);

    ASSERT_NE(texture, nullptr);

    ASSERT_EQ(texture->PixelWidth(), 342);
    ASSERT_EQ(texture->PixelHeight(), 342);

    Destroy();
}

TEST(TestTexture, show_image)
{
    Init();
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);
    ASSERT_NE(texture, nullptr);

    Vect flip = {1, 1};

    int r = texture->Render(nullptr, nullptr, 0, nullptr, &flip);
    ASSERT_EQ(r, 0);

    SDL_RenderPresent(g_renderer);

    SDL_Delay(5 * FRAME_DELAY);

    Destroy();
}

TEST(TestTexture, set_color_key)
{
    Init();

    Color colorkey = {255, 255, 255};
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, &colorkey);
    ASSERT_NE(texture, nullptr);

    Vect flip = {1, 1};

    int r = texture->Render(nullptr, nullptr, 0, nullptr, &flip);
    ASSERT_EQ(r, 0);

    SDL_RenderPresent(g_renderer);

    SDL_Delay(5 * FRAME_DELAY);

    Destroy();
}

TEST(TestTexture, set_color_alpha)
{
    int r;

    Init();
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);
    ASSERT_NE(texture, nullptr);

    Vect flip = {1, 1};

    for (int i = 0; i <= 255; i += 50)
    {
        SDL_RenderClear(g_renderer);

        ColorAlpha coloralpha = {255, 255, (u_char)i, 255};
        r = texture->SetColorAlpha(&coloralpha);
        ASSERT_EQ(r, 0);

        r = texture->Render(nullptr, nullptr, 0, nullptr, &flip);
        ASSERT_EQ(r, 0);

        SDL_RenderPresent(g_renderer);

        SDL_Delay(FRAME_DELAY);
    }

    for (int i = 255; i >= 0; i -= 50)
    {
        SDL_RenderClear(g_renderer);

        ColorAlpha coloralpha = {255, 255, 255, (u_char)i};
        r = texture->SetColorAlpha(&coloralpha);
        ASSERT_EQ(r, 0);

        r = texture->Render(nullptr, nullptr, 0, nullptr, &flip);
        ASSERT_EQ(r, 0);

        SDL_RenderPresent(g_renderer);

        SDL_Delay(FRAME_DELAY);
    }

    Destroy();
}

void RenderFrame(Texture *texture, Rect *srcrect, Rect *dstrect, float angle, Vect *center, Vect *flip)
{
    SDL_RenderClear(g_renderer);

    int r = texture->Render(srcrect, dstrect, angle, center, flip);
    ASSERT_EQ(r, 0);

    SDL_RenderPresent(g_renderer);

    SDL_Delay(FRAME_DELAY);
}

TEST(TestTexture, texture_render)
{
    Init();
    
    auto texture = Texture::LoadTexture(SOURCE_PATH, nullptr);
    ASSERT_NE(texture, nullptr);

    Vect flip1 = {1, 1};

    // normal
    RenderFrame(texture, nullptr, nullptr, 0, nullptr, &flip1);

    // window right part
    Rect dstrect1 = {400, 0, 400, 600};
    RenderFrame(texture, nullptr, &dstrect1, 0, nullptr, &flip1);

    // window center
    Rect dstrect2 = {300, 200, 200, 200};
    RenderFrame(texture, nullptr, &dstrect2, 0, nullptr, &flip1);

    // texture top-left to window center
    Rect srcrect1 = IRect{0, 0, texture->PixelWidth() / 2, texture->PixelHeight() / 2};
    RenderFrame(texture, &srcrect1, &dstrect2, 0, nullptr, nullptr);

    // texture left half
    Rect srcrect2 = IRect{0, 0, texture->PixelWidth() / 2, texture->PixelHeight()};
    Rect dstrect3 = {200, 100, (float)texture->PixelWidth() / 2, (float)texture->PixelHeight()};
    RenderFrame(texture, &srcrect2, &dstrect2, 0, nullptr, nullptr);

    // center rotate 30 degrees
    RenderFrame(texture, &srcrect2, &dstrect2, 30, nullptr, nullptr);

    // corner rotate 30 degrees
    Vect center1 = {0, 0};
    RenderFrame(texture, &srcrect2, &dstrect2, 30, &center1, nullptr);

    // flip horizantial
    Vect flip2 = {-1, 1};
    RenderFrame(texture, &srcrect2, &dstrect2, 0, nullptr, &flip2);

    // center rotate 30 degrees, again
    RenderFrame(texture, &srcrect2, &dstrect2, 30, nullptr, nullptr);

    // center rotate 30 and flip horizantial
    RenderFrame(texture, &srcrect2, &dstrect2, 30, nullptr, &flip2);

    // corner rotate 30 degrees, again
    RenderFrame(texture, &srcrect2, &dstrect2, 30, &center1, nullptr);

    // corner rotate 30 and flip horizential
    RenderFrame(texture, &srcrect2, &dstrect2, 30, &center1, &flip2);

    Destroy();
}

