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

    Matrix m2, m3;
    m2.AsScale({1, 1});
    m3.AsScale({1, 1});
    auto m4 = m2 * m3;
    ASSERT_FLOAT_EQ(m4._11, 1);
    ASSERT_FLOAT_EQ(m4._12, 0);
    ASSERT_FLOAT_EQ(m4._21, 0);
    ASSERT_FLOAT_EQ(m4._22, 1);
}

TEST(TestType, test_rect)
{
    Rect r1{1, 2, 3, 4};
    ASSERT_FLOAT_EQ(r1.x, 1);
    ASSERT_FLOAT_EQ(r1.y, 2);
    ASSERT_FLOAT_EQ(r1.w, 3);
    ASSERT_FLOAT_EQ(r1.h, 4);
}

TEST(TestType, test_cast)
{
    Vect v1{1, 1};

    Vect p = v1;
    ASSERT_FLOAT_EQ(p.x, v1.x);
    ASSERT_FLOAT_EQ(p.y, v1.y);

    b2Vec2 q = v1;
    ASSERT_FLOAT_EQ(q.x, v1.x);
    ASSERT_FLOAT_EQ(q.y, v1.y);

    Rect r1{1, 2, 3, 4};
    SDL_FRect r2 = r1;
    ASSERT_FLOAT_EQ(r2.x, 1);
    ASSERT_FLOAT_EQ(r2.y, 2);
    ASSERT_FLOAT_EQ(r2.w, 3);
    ASSERT_FLOAT_EQ(r2.h, 4);

    SDL_Rect r3 = r1;
    ASSERT_EQ(r3.x, 1);
    ASSERT_EQ(r3.y, 2);
    ASSERT_EQ(r3.w, 3);
    ASSERT_EQ(r3.h, 4);
}
