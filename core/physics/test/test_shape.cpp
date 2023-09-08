#include "gtest/gtest.h"
#include "physics.h"
#include <math.h>

using namespace gamo;

TEST(TestShape, test_circle)
{
    auto c = Circle();

    // just default
    auto b2c = (b2CircleShape *)c.ToBox2DShape({0, 0}, 0, {1, 1})[0];
    ASSERT_FLOAT_EQ(b2c->m_radius, 1);
    ASSERT_FLOAT_EQ(b2c->m_p.x, 0);
    ASSERT_FLOAT_EQ(b2c->m_p.y, 0);

    // change radius
    c.Radius(10);
    b2c = (b2CircleShape *)c.ToBox2DShape({0, 0}, 0, {1, 1})[0];
    ASSERT_FLOAT_EQ(b2c->m_radius, 10);
    ASSERT_FLOAT_EQ(b2c->m_p.x, 0);
    ASSERT_FLOAT_EQ(b2c->m_p.y, 0);

    // change center
    c.Center({2, 3});
    b2c = (b2CircleShape *)c.ToBox2DShape({0, 0}, 0, {1, 1})[0];
    ASSERT_FLOAT_EQ(b2c->m_radius, 10);
    ASSERT_FLOAT_EQ(b2c->m_p.x, -2);
    ASSERT_FLOAT_EQ(b2c->m_p.y, -3);

    // change offset
    b2c = (b2CircleShape *)c.ToBox2DShape({3, 2}, 0, {1, 1})[0];
    ASSERT_FLOAT_EQ(b2c->m_radius, 10);
    ASSERT_FLOAT_EQ(b2c->m_p.x, 1);
    ASSERT_FLOAT_EQ(b2c->m_p.y, -1);

    // change rotate
    c.Center({2, 0});
    b2c = (b2CircleShape *)c.ToBox2DShape({3, 2}, M_PI / 6, {1, 1})[0];
    ASSERT_FLOAT_EQ(b2c->m_radius, 10);
    float expect_x = 3 - sqrt(3);
    ASSERT_FLOAT_EQ(b2c->m_p.x, expect_x);
    ASSERT_FLOAT_EQ(b2c->m_p.y, 1);

    // change scale
    b2c = (b2CircleShape *)c.ToBox2DShape({3, 2}, M_PI / 6, {2, 5})[0];
    ASSERT_FLOAT_EQ(b2c->m_radius, 20);
    ASSERT_FLOAT_EQ(b2c->m_p.x, expect_x);
    ASSERT_FLOAT_EQ(b2c->m_p.y, 1);
}

bool Vec2In(b2Vec2 vects[], int count, b2Vec2 expect)
{
    for (int i = 0; i < count; i++)
    {
        auto vect = vects[i];
        if (fabs(vect.x - expect.x) < 1e-6 && fabs(vect.y - expect.y) < 1e-6)
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
    auto b2p = (b2PolygonShape *)p.ToBox2DShape({0, 0}, 0, {1, 1})[0];
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0.5, 0.5}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-0.5, 0.5}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-0.5, -0.5}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0.5, -0.5}));

    // change center
    p.Center({0.5, 0.5});
    b2p = (b2PolygonShape *)p.ToBox2DShape({0, 0}, 0, {1, 1})[0];
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0, 0}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-1, 0}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-1, -1}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0, -1}));

    // change offset
    b2p = (b2PolygonShape *)p.ToBox2DShape({1, 2}, 0, {1, 1})[0];
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {0, 1}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 1}));

    // change rotate
    b2p = (b2PolygonShape *)p.ToBox2DShape({1, 2}, M_PI / 4, {1, 1})[0];
    auto tmp = sqrtf(2) / 2;
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1 - tmp, 2 - tmp}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1 + tmp, 2 - tmp}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 2 - 2 * tmp}));

    // change scale
    b2p = (b2PolygonShape *)p.ToBox2DShape({1, 2}, 0, {2, 3})[0];
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {1, -1}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-1, 2}));
    ASSERT_TRUE(Vec2In(b2p->m_vertices, 4, {-1, -1}));

}

bool MatchPolygon(std::vector<Vect> vs, std::vector<Vect> expect)
{
    for (auto p : expect)
    {
        if (std::find(vs.begin(), vs.end(), p) == vs.end())
            return false;
    }
    return true;
}

bool ContainPolygon(std::vector<b2Shape *> list, std::vector<Vect> p)
{
    for (auto item : list)
    {
        auto tmp = ((b2ChainShape *)item);
        std::vector<Vect> vs;
        for (int i = 0; i < tmp->m_count; i++)
            vs.push_back({tmp->m_vertices[i].x, tmp->m_vertices[i].y});
        if (MatchPolygon(vs, p))
            return true;
    }
    return false;
}

TEST(TestShape, test_terrain)
{
    Terrain t1;
    t1.SetAsBox(0.5, 0.5, {0, 0});
    Terrain t2({{-1, 0}, {1, 0}, {1, 1}, {0.5, 1}, {0, 0.5}, {-0.5, 1}, {-1, 1}});
    Terrain t3;
    t3.SetAsBox(0.5, 0.5, {0, 0});

    TerrainPool pool;
    pool.Register(&t1);
    pool.Register(&t2);
    pool.Register(&t3);

    auto result = t1.ToBox2DShape({1.5, 1.5}, 0, {1, 1});
    ASSERT_EQ(result.size(), 0);

    result = t2.ToBox2DShape({4, 0}, 0, {1, 1});
    ASSERT_EQ(result.size(), 0);

    result = t3.ToBox2DShape({4, 1.5}, 0, {2, 1});
    ASSERT_EQ(result.size(), 3);

    ContainPolygon(result, {{1, 1}, {2, 1}, {2, 2}, {1, 2}});
    ContainPolygon(result, {{3, 0}, {5, 0}, {5, 2}, {3, 2}});
    ContainPolygon(result, {{4, 0.5}, {3.5, 1}, {4.5, 1}});
}
