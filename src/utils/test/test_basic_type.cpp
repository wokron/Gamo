#include "gtest/gtest.h"
#include "utils.h"

using namespace gamo;

TEST(TestType, test_vector)
{
    Vect v1{1, 1};
    ASSERT_FLOAT_EQ(v1.x, 1);
    ASSERT_FLOAT_EQ(v1.y, 1);

    Vect v2{2, 2};
    auto v3 = v1 + v2;
    ASSERT_FLOAT_EQ(v3.x, 3);
    ASSERT_FLOAT_EQ(v3.y, 3);

    auto v4 = v3 * v2;
    ASSERT_FLOAT_EQ(v4.x, 6);
    ASSERT_FLOAT_EQ(v4.y, 6);

    auto v5 = v4 * 3;
    ASSERT_FLOAT_EQ(v5.x, 18);
    ASSERT_FLOAT_EQ(v5.y, 18);

    auto v6 = -v5;
    ASSERT_FLOAT_EQ(v6.x, -18);
    ASSERT_FLOAT_EQ(v6.y, -18);

    auto v7 = v6 / -18;
    ASSERT_FLOAT_EQ(v7.x, 1);
    ASSERT_FLOAT_EQ(v7.y, 1);

    auto sdl = v7.ToSDL();
    ASSERT_FLOAT_EQ(sdl.x, 1);
    ASSERT_FLOAT_EQ(sdl.y, 1);

    auto b2 = v7.ToBox2d();
    ASSERT_FLOAT_EQ(b2.x, 1);
    ASSERT_FLOAT_EQ(b2.y, 1);
}

TEST(TestType, test_matrix)
{
    Vect v1{2, 3};
    Matrix m1{2, 3, 1, 2};

    auto v_ = m1 * v1;
    ASSERT_FLOAT_EQ(v_.x, 13);
    ASSERT_FLOAT_EQ(v_.y, 8);

    Vect v2{1, 0};
    Matrix r;
    r.AsRotate(M_PI / 6);
    auto v3 = r * v2;
    float x = cosf(M_PI / 6);
    float y = sinf(M_PI / 6);
    ASSERT_FLOAT_EQ(v3.x, x);
    ASSERT_FLOAT_EQ(v3.y, y);

    Vect v4{3, 8};
    Matrix s;
    s.AsScale({3, 5});
    auto v5 = s * v4;
    ASSERT_FLOAT_EQ(v5.x, 9);
    ASSERT_FLOAT_EQ(v5.y, 40);

    Vect v6{1, 0};
    Matrix r1, s1;
    r1.AsRotate(M_PI / 6);
    s1.AsScale({2, 1});
    auto v7 = r1 * s1 * v6;
    x = 2 * cosf(M_PI / 6);
    y = 2 * sinf(M_PI / 6);
    ASSERT_FLOAT_EQ(v7.x, x);
    ASSERT_FLOAT_EQ(v7.y, y);

    auto v8 = s1 * r1 * v6;
    x = 2 * cosf(M_PI / 6);
    y = sinf(M_PI / 6);
    ASSERT_FLOAT_EQ(v8.x, x);
    ASSERT_FLOAT_EQ(v8.y, y);
}

TEST(TestType, test_rect)
{
    Rect r1{1, 2, 3, 4};
    ASSERT_FLOAT_EQ(r1.xy.x, 1);
    ASSERT_FLOAT_EQ(r1.xy.y, 2);
    ASSERT_FLOAT_EQ(r1.wh.x, 3);
    ASSERT_FLOAT_EQ(r1.wh.y, 4);

    auto i = r1.ToSDL();
    ASSERT_EQ(i.x, 1);
    ASSERT_EQ(i.y, 2);
    ASSERT_EQ(i.w, 3);
    ASSERT_EQ(i.h, 4);

    auto f = r1.ToSDL_F();
    ASSERT_FLOAT_EQ(f.x, 1);
    ASSERT_FLOAT_EQ(f.y, 2);
    ASSERT_FLOAT_EQ(f.w, 3);
    ASSERT_FLOAT_EQ(f.h, 4);
}
