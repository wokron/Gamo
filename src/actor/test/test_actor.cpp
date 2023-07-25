#include "gtest/gtest.h"
#include "actor.h"

using namespace gamo;

TEST(TestActor, create_actor)
{
    SDL_FPoint position = {0, 0};
    SDL_FPoint scale = {1, 1};
    auto actor = new Actor({0, 0}, 0, {1, 1});
    ASSERT_NE(actor, nullptr);
    ASSERT_EQ(actor->SupActor(), nullptr);

    ASSERT_EQ(actor->GetCharacteristics().size(), 1);
    ASSERT_NE(actor->GetTransform(), nullptr);
    ASSERT_NE(actor->GetCharacteristicByType("Transform"), nullptr);
}

TEST(TestActor, sub_actors)
{
    SDL_FPoint position = {0, 0};
    SDL_FPoint scale = {1, 1};
    auto actor = new Actor({0, 0}, 0, {1, 1});

    Actor *subs[10];
    for (int i = 0; i < 10; i++)
    {
        subs[i] = actor->CreateSubActor({0, 0}, 0, {1, 1});
        ASSERT_EQ(subs[i]->GetCharacteristics().size(), 1);
        ASSERT_NE(subs[i]->GetTransform(), nullptr);
        ASSERT_NE(subs[i]->GetCharacteristicByType("Transform"), nullptr);
    }

    ASSERT_EQ(actor->SubActors().size(), 10);
    for (int i = 0; i < 10; i++)
    {
        ASSERT_EQ(subs[i]->SupActor(), actor);
    }

    auto actor2 = subs[5];

    Actor *subs2[20];
    for (int i = 0; i < 20; i++)
    {
        subs2[i] = actor2->CreateSubActor({0, 0}, 0, {1, 1});
        ASSERT_EQ(subs2[i]->GetCharacteristics().size(), 1);
        ASSERT_NE(subs2[i]->GetTransform(), nullptr);
        ASSERT_NE(subs2[i]->GetCharacteristicByType("Transform"), nullptr);
    }

    ASSERT_EQ(subs2[10]->SupActor(), actor2);
    ASSERT_EQ(subs2[5]->SupActor()->SupActor(), actor);
    ASSERT_EQ(subs2[1]->SupActor()->SupActor()->SupActor(), nullptr);
}

TEST(TestActor, characteristic_in_actor)
{
    auto actor = new Actor({0, 0}, 0, {1, 1});

    ASSERT_EQ(actor->GetCharacteristics().size(), 1);
    ASSERT_NE(actor->GetTransform(), nullptr);
    ASSERT_NE(actor->GetCharacteristicByType("Transform"), nullptr);
    ASSERT_EQ(actor->GetTransform(), actor->GetCharacteristicByType("Transform"));

    ASSERT_EQ(actor->GetCharacteristicByType("Renderer"), nullptr);
    ASSERT_EQ(actor->GetCharacteristicByType("Camera"), nullptr);
    ASSERT_EQ(actor->GetCharacteristicByType("Any Name"), nullptr);

    auto t = actor->GetTransform();
    ASSERT_EQ(t->BelongActor(), actor);

    actor->GetCharacteristics().push_back(new Renderer(actor));

    ASSERT_EQ(actor->GetCharacteristics().size(), 2);

    ASSERT_EQ(actor->GetCharacteristicByType("Camera"), nullptr);
    ASSERT_EQ(actor->GetCharacteristicByType("Any Name"), nullptr);

    actor->GetCharacteristics().push_back(new Camera(actor));

    ASSERT_EQ(actor->GetCharacteristics().size(), 3);

    ASSERT_EQ(actor->GetCharacteristicByType("Any Name"), nullptr);
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

TEST(TestActor, test_event_register)
{
    auto actor = new Actor({0, 0}, 0, {1, 1});
    auto b = new EventBehavior(actor);
    actor->GetCharacteristics().push_back(b);

    auto sub_actor = actor->CreateSubActor({0, 0}, 0, {1, 1});
    auto b2 = new EventBehavior(sub_actor);
    sub_actor->GetCharacteristics().push_back(b2);

    actor->RegisterSystemEvents(); // register events in actor and subactor
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

    actor->UnregisterSystemEvents();
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
