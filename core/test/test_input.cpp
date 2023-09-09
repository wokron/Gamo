#include "gtest/gtest.h"
#include "gamo/game/game.h"
#include "gamo/render/render.h"
#include "gamo/input/input.h"
#include "spdlog/spdlog.h"

using namespace gamo;

TEST(TestInput, test_singleton)
{
    auto input = Input::GetInstance();
    ASSERT_NE(input, nullptr);

    auto input2 = Input::GetInstance();
    ASSERT_NE(input2, nullptr);
    ASSERT_EQ(input, input2);
}

TEST(TestInput, test_key)
{
    Play::GetInstance()->Init("test input", 400, 300, true);

    auto renderer = RenderAsset::GetInstance()->Renderer();
    auto input = Input::GetInstance();

    input->UpdateInputStates();
    while (!input->GetKeyDown(SDL_SCANCODE_A))
    {
        input->UpdateInputStates();
    }

    // red
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    input->UpdateInputStates();
    while (!input->GetKeyUp(SDL_SCANCODE_A))
    {
        input->UpdateInputStates();
    }

    // green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    for (int i = 0; i < 255; i++)
    {
        input->UpdateInputStates();
        while (!input->GetKey(SDL_SCANCODE_A))
        {
            input->UpdateInputStates();
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, i, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
    }

    input->UpdateInputStates();
    bool flag = true;
    while (!input->Quit())
    {
        // ctrl + a
        if (input->GetKey(SDL_SCANCODE_LCTRL) && input->GetKeyDown(SDL_SCANCODE_A))
        {
            flag = !flag;
        }

        if (flag)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(5);

        input->UpdateInputStates();
    }

    Play::GetInstance()->Destroy();
}

TEST(TestInput, test_mouse_position)
{
    Play::GetInstance()->Init("test input", 400, 300, true);

    auto renderer = RenderAsset::GetInstance()->Renderer();
    auto input = Input::GetInstance();

    input->UpdateInputStates();
    while (!input->Quit())
    {
        int x, y;
        input->GetMousePosition(&x, &y);
        if (input->GetMouseButton(MouseButton::Left))
        {
            SDL_SetRenderDrawColor(renderer, 0, (Uint8)(x / 400.0 * 255), (Uint8)(y / 400.0 * 255), 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 100, (Uint8)(x / 400.0 * 255), (Uint8)(y / 400.0 * 255), 255);
        }
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(50);

        input->UpdateInputStates();
    }

    Play::GetInstance()->Destroy();
}
