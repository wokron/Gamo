#include "gtest/gtest.h"
#include "gamo/gamo.h"

using namespace gamo;

class MyClass : public IPrototype<MyClass>
{
public:
    int val1 = 0;
    int val2 = 0;

    MyClass(int val1) : val1(val1) {}

    MyClass *Clone() override
    {
        return new MyClass(val1);
    }
};

TEST(TestPrototypeTemplate, test_prototype)
{
    auto obj = new MyClass(123);
    ASSERT_EQ(obj->val1, 123);
    ASSERT_EQ(obj->val2, 0);

    auto obj2 = obj->Clone();
    ASSERT_NE(obj, obj2);
    ASSERT_EQ(obj2->val1, 123);
    ASSERT_EQ(obj2->val2, 0);

    obj->val1 = 12;
    ASSERT_EQ(obj->val1, 12);
    ASSERT_EQ(obj->val2, 0);
    ASSERT_EQ(obj2->val1, 123);
    ASSERT_EQ(obj2->val2, 0);

}