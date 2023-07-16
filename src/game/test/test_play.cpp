#include "gtest/gtest.h"
#include "director.h"
#include "game.h"
#include "render.h"
#include "actor.h"
#include "SDL2/SDL_image.h"

using namespace gamo;


TEST(TestPlay, test_singleton)
{
    auto play = Play::GetInstance();
    ASSERT_NE(play, nullptr);

    auto play2 = Play::GetInstance();
    ASSERT_NE(play2, nullptr);
    ASSERT_EQ(play, play2);
}

TEST(TestPlay, test_init)
{
    auto play = Play::GetInstance();
    ASSERT_NE(play, nullptr);

    ASSERT_EQ(RenderAsset::GetInstance()->Window(), nullptr);
    ASSERT_EQ(RenderAsset::GetInstance()->Renderer(), nullptr);
    ASSERT_EQ(play->TargetFrameRate(), 0);
    
    int r = play->Init("test play init", 400, 300, true);
    ASSERT_EQ(r, 0);

    ASSERT_NE(RenderAsset::GetInstance()->Window(), nullptr);
    ASSERT_NE(RenderAsset::GetInstance()->Renderer(), nullptr);

    play->Destroy();
}

TEST(TestPlay, test_set_fps)
{
    auto play = Play::GetInstance();
    ASSERT_NE(play, nullptr);

    play->Init("test play set fps", 400, 300, true);

    ASSERT_NE(play->TargetFrameRate(), 0);
    auto tmp = play->TargetFrameRate();

    play->TargetFrameRate(10);
    ASSERT_EQ(play->TargetFrameRate(), 10);

    play->TargetFrameRate(-1);
    ASSERT_EQ(play->TargetFrameRate(), tmp);
}

TEST(TestPlay, test_scene)
{
    auto play = Play::GetInstance();
    ASSERT_EQ(play->CurrentScene(), nullptr);

    Scene *scenes[5];

    for (int i = 0; i < 5; i++)
    {
        scenes[i] = new Scene();
        play->AddScene(scenes[i]);
    }

    // 0
    play->PushScene(0);
    ASSERT_EQ(play->CurrentScene(), scenes[0]);

    // 0 1
    play->PushScene(1);
    ASSERT_EQ(play->CurrentScene(), scenes[1]);

    ASSERT_LT(play->PushScene(5), 0);

    // 0 4
    play->ReplaceScene(4);
    ASSERT_EQ(play->CurrentScene(), scenes[4]);

    // 0
    play->PopScene();
    ASSERT_EQ(play->CurrentScene(), scenes[0]);

    // 
    play->PopScene();
    ASSERT_EQ(play->CurrentScene(), nullptr);

    // 3
    play->ReplaceScene(3);
    ASSERT_EQ(play->CurrentScene(), scenes[3]);
}

// TEST(TestPlay, test_run)
// {
//     auto play = Play::GetInstance();
//     play->Init("test play test run", 800, 600, true);

//     auto scene = new Scene();
//     play->AddScene(scene);
    
//     auto actor = new Actor({0, 0}, 0, {1, 1});
//     scene->AddActor(actor);
//     auto camera = new Camera(actor);
//     actor->GetCharacteristics().push_back(camera);
//     auto renderer = new Renderer(actor);
//     renderer->TargetSprite()
//     actor->GetCharacteristics().push_back(renderer);


    
// }