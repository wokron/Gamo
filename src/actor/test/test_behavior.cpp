#include "gtest/gtest.h"
#include "actor.h"

using namespace gamo;

class EventBehavior : public Behavior
{
public:
    EventBehavior() : Behavior(new Actor({0, 0}, 0, {1, 1})) {}
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

TEST(TestBehavior, test_event_register)
{
    auto b1 = new EventBehavior();

    // on scene push/replace
    b1->RegisterHandleOnStart();
    b1->RegisterHandleOnUpdate();
    b1->RegisterHandleOnLateUpdate();
    EventDispatcher::GetInstance()->Dispatch(EVENT_ON_START, nullptr);

    // on main loop
    ASSERT_TRUE(b1->start);
    for (int i = 0; i < 10; i++)
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_UPDATE, nullptr);
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_LATE_UPDATE, nullptr);
        ASSERT_EQ(b1->update, i + 1);
        ASSERT_EQ(b1->late_update, i + 1);
    }

    auto b2 = new EventBehavior();

    // now another scene **cover/replace** the previous scene
    b1->UnregisterHandleOnStart();
    b1->UnregisterHandleOnUpdate();
    b1->UnregisterHandleOnLateUpdate();
    b2->RegisterHandleOnStart();
    b2->RegisterHandleOnUpdate();
    b2->RegisterHandleOnLateUpdate();
    EventDispatcher::GetInstance()->Dispatch(EVENT_ON_START, nullptr);

    // on main loop
    ASSERT_TRUE(b2->start);
    for (int i = 0; i < 10; i++)
    {
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_UPDATE, nullptr);
        EventDispatcher::GetInstance()->Dispatch(EVENT_ON_LATE_UPDATE, nullptr);
        ASSERT_EQ(b2->update, i + 1);
        ASSERT_EQ(b2->late_update, i + 1);
        // b1 will not update
        ASSERT_EQ(b1->update, 10);
        ASSERT_EQ(b1->late_update, 10);
    }
}
