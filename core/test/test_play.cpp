#include "gtest/gtest.h"
#include "director.h"
#include "game.h"
#include "render.h"
#include "actor.h"
#include "SDL2/SDL_image.h"
#include <thread>
#include "config.h"

const char *ANIMATE_PATH = RESOURCE_DIR "/walk_animate.png"; // this image come from Lazy Foo' Productions（http://lazyfoo.net/） 

using namespace gamo;

Sprite *GetAnimateSprite()
{
    Color colorkey = {0, 0xff, 0xff};
    auto texture = Texture::LoadTexture(ANIMATE_PATH, &colorkey);
    texture->PixelPerUnit(100);

    SingleSprite *sprites[4];

    int th = texture->PixelHeight(), tw = texture->PixelWidth();
    int w = tw / 4, h = th;

    for (int i = 0; i < 4; i++)
    {
        Rect cliprect = IRect{w * i, 0, w, h};
        sprites[i] = texture->ClipAndCreateSprite(&cliprect);
    }

    auto animation = new AnimateSprite();

    for (int i = 0; i < 4; i++)
    {
        animation->AddFrame(5, sprites[i]);
    }

    return animation;
}

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

    play->Destroy();
}

TEST(TestPlay, test_scene)
{
    auto play = Play::GetInstance();
    play->Init("test play", 400, 300, true);
    ASSERT_EQ(play->CurrentScene(), nullptr);

    Scene *scenes[5];

    for (int i = 0; i < 5; i++)
    {
        scenes[i] = new Scene();
    }

    // 0
    play->PushScene(scenes[0]);
    ASSERT_EQ(play->CurrentScene(), scenes[0]);

    // 0 1
    play->PushScene(scenes[1]);
    ASSERT_EQ(play->CurrentScene(), scenes[1]);

    // 0 4
    play->ReplaceScene(scenes[4]);
    ASSERT_EQ(play->CurrentScene(), scenes[4]);

    // 0
    play->PopScene();
    ASSERT_EQ(play->CurrentScene(), scenes[0]);

    // 
    play->PopScene();
    ASSERT_EQ(play->CurrentScene(), nullptr);

    // 3
    play->ReplaceScene(scenes[3]);
    ASSERT_EQ(play->CurrentScene(), scenes[3]);

    play->Destroy();
}

TEST(TestPlay, test_run)
{
    auto play = Play::GetInstance();
    play->Init("test play test run", 800, 600, true);

    auto scene = new Scene();

    auto camera_actor = new Actor({0, 0}, 0, {1, 1});
    scene->AddActor(camera_actor);
    auto camera = new Camera(camera_actor);
    camera->Layers(LAYER(0));
    camera_actor->GetCharacteristics().push_back(camera);
    
    auto actor = new Actor({0, 0}, 0, {1, 1});
    actor->Layer(LAYER(0));
    scene->AddActor(actor);
    auto renderer = new Renderer(actor);
    renderer->TargetSprite(GetAnimateSprite());
    actor->GetCharacteristics().push_back(renderer);

    SDL_SetRenderDrawColor(RenderAsset::GetInstance()->Renderer(), 255, 255, 255, 255);

    play->PushScene(scene);

    std::thread t_quit = std::thread([actor](){
        for (float i = -2; i < 2; i += 0.1)
        {
            actor->GetTransform()->Position({-i, 0});
            SDL_Delay(50);
        }
        Play::GetInstance()->Quit(true);
    });

    play->Perform();

    t_quit.join();

    play->Destroy();
}

class MoveBehavior2 : public Behavior
{
public:
    MoveBehavior2(Actor *actor) : Behavior(actor) {}
    bool start = false;
    void OnStart()
    {
        ASSERT_FALSE(start);
        start = true;
    }
    
    int update = 0;
    float pos = -2;
    void OnUpdate()
    {
        ASSERT_TRUE(start);
        BelongActor()->GetTransform()->Position({-pos, 0});
        pos += 0.05;
        update++;
        if (pos >= 2)
        {
            Play::GetInstance()->Quit(true);
        }
    }

    int late_update = 0;
    void OnLateUpdate()
    {
        ASSERT_EQ(update - 1, late_update);
        late_update++;
    }
};

class MoveBehavior : public Behavior
{
public:
    MoveBehavior(Actor *actor) : Behavior(actor) {}
    bool start = false;
    void OnStart()
    {
        ASSERT_FALSE(start);
        start = true;
    }

    int update = 0;
    float pos = -2;
    void OnUpdate()
    {
        ASSERT_TRUE(start);
        BelongActor()->GetTransform()->Position({pos, 0});
        pos += 0.05;
        update++;
        if (pos >= 2)
        {
            auto scene = new Scene();

            auto camera_actor = new Actor({0, 0}, 0, {1, 1});
            scene->AddActor(camera_actor);
            auto camera = new Camera(camera_actor);
            camera->Layers(LAYER(0));
            camera_actor->GetCharacteristics().push_back(camera);
            
            auto actor = new Actor({0, 0}, 0, {1, 1});
            actor->Layer(LAYER(0));
            scene->AddActor(actor);
            auto renderer = new Renderer(actor);
            renderer->TargetSprite(GetAnimateSprite());
            actor->GetCharacteristics().push_back(renderer);
            actor->GetCharacteristics().push_back(new MoveBehavior2(actor));

            Play::GetInstance()->ReplaceScene(scene);
        }
    }

    int late_update = 0;
    void OnLateUpdate()
    {
        ASSERT_EQ(update - 1, late_update);
        late_update++;
    }
};

TEST(TestPlay, test_behavior)
{
    auto play = Play::GetInstance();
    play->Quit(false);
    play->Init("test play test run", 800, 600, true);

    auto scene = new Scene();

    auto camera_actor = new Actor({0, 0}, 0, {1, 1});
    scene->AddActor(camera_actor);
    auto camera = new Camera(camera_actor);
    camera->Layers(LAYER(0));
    camera_actor->GetCharacteristics().push_back(camera);
    
    auto actor = new Actor({0, 0}, 0, {-1, 1});
    actor->Layer(LAYER(0));
    scene->AddActor(actor);
    auto renderer = new Renderer(actor);
    renderer->TargetSprite(GetAnimateSprite());
    actor->GetCharacteristics().push_back(renderer);
    actor->GetCharacteristics().push_back(new MoveBehavior(actor));

    SDL_SetRenderDrawColor(RenderAsset::GetInstance()->Renderer(), 255, 255, 255, 255);

    play->PushScene(scene);

    play->Perform();

    play->Destroy();
}
