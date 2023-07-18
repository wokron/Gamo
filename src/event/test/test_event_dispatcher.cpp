#include "gtest/gtest.h"
#include "event.h"
#include <algorithm>
#include <functional>

using namespace gamo;

TEST(TestEventDispatcher, test_singleton)
{
    auto dispatcher1 = EventDispatcher::GetInstance();
    ASSERT_NE(dispatcher1, nullptr);

    auto dispatcher2 = EventDispatcher::GetInstance();
    ASSERT_NE(dispatcher2, nullptr);
    ASSERT_EQ(dispatcher1, dispatcher2);
}

class TestEvent : public Event
{
public:
    TestEvent(int no) : _no(no) {}
    int _no;
    virtual int Type() const { return 1; }

};

class TestEventCount
{
public:
    int count[10] = {0};
    void clear()
    {
        memset(count, 0, sizeof(count));
    }

    void receive(Event *e)
    {
        auto te = (TestEvent *)e;
        count[te->_no]++;
    }
};

TEST(TestEventDispatcher, test_single)
{
    TestEventCount count;
    auto dispatcher = EventDispatcher::GetInstance();
    
    dispatcher->Append(1, std::bind(&TestEventCount::receive, &count, std::placeholders::_1));

    TestEvent e0(0);
    dispatcher->Dispatch(&e0);
    TestEvent e1(1);
    dispatcher->Dispatch(&e1);
    dispatcher->Dispatch(&e0);

    ASSERT_EQ(count.count[0], 2);
    ASSERT_EQ(count.count[1], 1);
}

TEST(TestEventDispatcher, test_multiple)
{
    TestEventCount count;
    TestEventCount count2;
    auto dispatcher = EventDispatcher::GetInstance();
    
    auto handle1 = dispatcher->Append(1, std::bind(&TestEventCount::receive, &count, std::placeholders::_1));
    auto handle2 = dispatcher->Append(1, std::bind(&TestEventCount::receive, &count, std::placeholders::_1));
    auto handle3 = dispatcher->Append(1, std::bind(&TestEventCount::receive, &count2, std::placeholders::_1));

    TestEvent e0(0);
    dispatcher->Dispatch(&e0);
    TestEvent e1(1);
    dispatcher->Dispatch(&e1);
    dispatcher->Dispatch(&e0);

    ASSERT_EQ(count.count[0], 4);
    ASSERT_EQ(count.count[1], 2);
    ASSERT_EQ(count2.count[0], 2);
    ASSERT_EQ(count2.count[1], 1);

    // remove all listeners in event 1
    dispatcher->Remove(1, handle1);
    dispatcher->Remove(1, handle2);
    dispatcher->Remove(1, handle3);

    dispatcher->Dispatch(&e0);
    dispatcher->Dispatch(&e1);
    dispatcher->Dispatch(&e0);

    ASSERT_EQ(count.count[0], 4);
    ASSERT_EQ(count.count[1], 2);
    ASSERT_EQ(count2.count[0], 2);
    ASSERT_EQ(count2.count[1], 1);

    // dispatch another event
    dispatcher->Append(2, std::bind(&TestEventCount::receive, &count, std::placeholders::_1));
    TestEvent e2(2);
    dispatcher->Dispatch(2, &e2);
    ASSERT_EQ(count.count[2], 1);
}

TEST(TestEventDispatcher, test_macro)
{
    TestEventCount count;
    TestEventCount count2;
    auto dispatcher = EventDispatcher::GetInstance();
    
    dispatcher->Append(3, MEMBER_METHOD(&count, &TestEventCount::receive));
    dispatcher->Append(3, MEMBER_METHOD(&count, &TestEventCount::receive));
    dispatcher->Append(3, MEMBER_METHOD(&count2, &TestEventCount::receive));

    TestEvent e0(0);
    dispatcher->Dispatch(3, &e0);
    TestEvent e1(1);
    dispatcher->Dispatch(3, &e1);
    dispatcher->Dispatch(3, &e0);

    ASSERT_EQ(count.count[0], 4);
    ASSERT_EQ(count.count[1], 2);
    ASSERT_EQ(count2.count[0], 2);
    ASSERT_EQ(count2.count[1], 1);
}

class TestNull
{
public:
    bool is_null = false;
    void receive(Event *e)
    {
        is_null = (e == nullptr);
    }
};

TEST(TestEventDispatcher, test_null)
{
    TestNull test;
    auto dispatcher = EventDispatcher::GetInstance();

    dispatcher->Append(10, MEMBER_METHOD(&test, &TestNull::receive));

    ASSERT_FALSE(test.is_null);
    dispatcher->Dispatch(10, nullptr);
    ASSERT_TRUE(test.is_null);
}
