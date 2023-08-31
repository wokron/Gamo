#include "gtest/gtest.h"
#include "physics.h"
#include <math.h>

using namespace gamo;

TEST(TestShape, test_circle)
{
    auto c = Circle();

    // just default
    auto b2c = (b2CircleShape *)c.ToBox2DShape({0, 0}, 0, {1, 1});
    ASSERT_FLOAT_EQ(b2c->m_radius, 1);
    ASSERT_FLOAT_EQ(b2c->m_p.x, 0);
    ASSERT_FLOAT_EQ(b2c->m_p.y, 0);

    // change radius
    c.Radius(10);
    b2c = (b2CircleShape *)c.ToBox2DShape({0, 0}, 0, {1, 1});
    ASSERT_FLOAT_EQ(b2c->m_radius, 10);
    ASSERT_FLOAT_EQ(b2c->m_p.x, 0);
    ASSERT_FLOAT_EQ(b2c->m_p.y, 0);

    // change center
    c.Center({2, 3});
    b2c = (b2CircleShape *)c.ToBox2DShape({0, 0}, 0, {1, 1});
    ASSERT_FLOAT_EQ(b2c->m_radius, 10);
    ASSERT_FLOAT_EQ(b2c->m_p.x, -2);
    ASSERT_FLOAT_EQ(b2c->m_p.y, -3);

    // change offset
    b2c = (b2CircleShape *)c.ToBox2DShape({3, 2}, 0, {1, 1});
    ASSERT_FLOAT_EQ(b2c->m_radius, 10);
    ASSERT_FLOAT_EQ(b2c->m_p.x, 1);
    ASSERT_FLOAT_EQ(b2c->m_p.y, -1);

    // change rotate
    c.Center({2, 0});
    b2c = (b2CircleShape *)c.ToBox2DShape({3, 2}, M_PI / 6, {1, 1});
    ASSERT_FLOAT_EQ(b2c->m_radius, 10);
    float expect_x = 3 - sqrt(3);
    ASSERT_FLOAT_EQ(b2c->m_p.x, expect_x);
    ASSERT_FLOAT_EQ(b2c->m_p.y, 1);

    // change scale
    b2c = (b2CircleShape *)c.ToBox2DShape({3, 2}, M_PI / 6, {2, 5});
    ASSERT_FLOAT_EQ(b2c->m_radius, 20);
    ASSERT_FLOAT_EQ(b2c->m_p.x, expect_x);
    ASSERT_FLOAT_EQ(b2c->m_p.y, 1);
}

bool Vec2In(b2Vec2 vects[], int count, b2Vec2 expect)
{
    for (int i = 0; i < count; i++)
    {
        auto vect = vects[i];
        if (fabs(vect.x - expect.x) < 1e-2 && fabs(vect.y - expect.y) < 1e-2)
        {
            return true;
        }
    }

    return false;
}

TEST(TestShape, test_polygon)
{
    auto p = Polygon();

    // just default
    auto b2p = (b2PolygonShape *)p.ToBox2DShape({0, 0}, 0, {1, 1});
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0.5, 0.5}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-0.5, 0.5}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-0.5, -0.5}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0.5, -0.5}));

    // change center
    p.Center({0.5, 0.5});
    b2p = (b2PolygonShape *)p.ToBox2DShape({0, 0}, 0, {1, 1});
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0, 0}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-1, 0}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-1, -1}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0, -1}));

    // change offset
    b2p = (b2PolygonShape *)p.ToBox2DShape({1, 2}, 0, {1, 1});
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0, 1}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 1}));

    // change rotate
    b2p = (b2PolygonShape *)p.ToBox2DShape({1, 2}, M_PI / 4, {1, 1});
    auto tmp = sqrtf(2) / 2;
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1 - tmp, 2 - tmp}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1 + tmp, 2 - tmp}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 2 - 2 * tmp}));

    // change scale
    b2p = (b2PolygonShape *)p.ToBox2DShape({1, 2}, 0, {2, 3});
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, -1}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-1, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-1, -1}));

}