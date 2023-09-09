#include "gtest/gtest.h"
#include "actor.h"

using namespace gamo;

TEST(TestTransform, test_global)
{
    auto a1 = new Actor({2, 3}, 0, {1, 1});
    auto a2 = a1->CreateSubActor({1, 1}, 30, {1, 2});
    auto a3 = a2->CreateSubActor({1, 0}, 60, {2, 1});

    auto pos1 = a1->GetTransform()->GlobalPosition();
    auto r1 = a1->GetTransform()->GlobalRotate();
    auto s1 = a1->GetTransform()->GlobalScale();
    ASSERT_FLOAT_EQ(pos1.x, 2);
    ASSERT_FLOAT_EQ(pos1.y, 3);
    ASSERT_FLOAT_EQ(r1, 0);
    ASSERT_FLOAT_EQ(s1.x, 1);
    ASSERT_FLOAT_EQ(s1.y, 1);

    auto pos2 = a2->GetTransform()->GlobalPosition();
    auto r2 = a2->GetTransform()->GlobalRotate();
    auto s2 = a2->GetTransform()->GlobalScale();
    ASSERT_FLOAT_EQ(pos2.x, 3);
    ASSERT_FLOAT_EQ(pos2.y, 4);
    ASSERT_FLOAT_EQ(r2, 30);
    ASSERT_FLOAT_EQ(s2.x, 1);
    ASSERT_FLOAT_EQ(s2.y, 2);

    auto pos3 = a3->GetTransform()->GlobalPosition();
    auto r3 = a3->GetTransform()->GlobalRotate();
    auto s3 = a3->GetTransform()->GlobalScale();
    ASSERT_FLOAT_EQ(pos3.x, 3 + sqrt(3) / 2);
    ASSERT_FLOAT_EQ(pos3.y, 4 + 0.5);
    ASSERT_FLOAT_EQ(r3, 90);
    ASSERT_FLOAT_EQ(s3.x, 2);
    ASSERT_FLOAT_EQ(s3.y, 2);
}