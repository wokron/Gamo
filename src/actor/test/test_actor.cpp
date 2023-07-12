#include "gtest/gtest.h"
#include "actor.h"

using namespace gamo;

TEST(TestActor, create_actor)
{
    FPoint position = {0, 0};
    FVect scale = {1, 1};
    auto actor = new Actor({0, 0}, 0, {1, 1});
    ASSERT_NE(actor, nullptr);
    ASSERT_EQ(actor->SupActor(), nullptr);

    ASSERT_EQ(actor->GetCharacteristics().size(), 1);
    ASSERT_NE(actor->GetTransform(), nullptr);
    ASSERT_NE(actor->GetCharacteristicByType("Transform"), nullptr);
}

TEST(TestActor, sub_actors)
{
    FPoint position = {0, 0};
    FVect scale = {1, 1};
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
    FPoint position = {0, 0};
    FVect scale = {1, 1};
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
