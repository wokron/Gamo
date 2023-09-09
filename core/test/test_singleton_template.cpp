#include "gtest/gtest.h"
#include "gamo/utils/singleton.h"

using namespace gamo;

SINGLETON_BEGIN(MySingleton)
public:
    int value = 0;
    bool started = false;

    void Init() { started = true; }
    void Destroy() { started = false; }
SINGLETON_END

TEST(TestSingletonTemplate, test_singleton)
{
    auto a = MySingleton::GetInstance();
    ASSERT_NE(a, nullptr);
    auto b = MySingleton::GetInstance();
    ASSERT_NE(b, nullptr);
    ASSERT_EQ(a, b);

    a->value = 10;
    ASSERT_EQ(b->value, 10);

    ASSERT_FALSE(b->started);
    a->Init();
    ASSERT_TRUE(b->started);
    b->Destroy();
    ASSERT_FALSE(a->started);
}
