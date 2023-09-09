#include "gtest/gtest.h"
#include "gamo/algo/algo.h"

using namespace gamo;

bool MatchPolygon(AlgoPolygon actual, AlgoPolygon expect)
{
    for (auto p : expect)
    {
        if (std::find(actual.begin(), actual.end(), p) == actual.end())
            return false;
    }
    return true;
}

bool ContainPolygon(PolygonList list, AlgoPolygon p)
{
    for (auto item : list)
    {
        if (MatchPolygon(item, p))
            return true;
    }
    return false;
}

bool IsConvex(AlgoPolygon polygon)
{
    int n = polygon.size();
    polygon.push_back(polygon[0]);
    polygon.push_back(polygon[1]);
    for (int i = 0; i < n; i++)
    {
        Vect v1 = polygon[i + 1] - polygon[i];
        Vect v2 = polygon[i + 2] - polygon[i + 1];

        if (v1.x * v2.y - v2.x * v1.y < 0)
        {
            return false;
        }
    }
    return true;
}

TEST(TestPolygonAlgo, test_compose)
{
    AlgoPolygon p1 = {{1, 1}, {2, 1}, {2, 2}, {1, 2}};
    AlgoPolygon p2 = {{1, 2}, {2, 2}, {2, 3}, {1, 3}};
    AlgoPolygon p3 = {{3, 1}, {5, 1}, {5, 3}, {4.5, 3}, {4, 2}, {3.5, 3}, {3, 3}};
    AlgoPolygon p4 = {{3, 3}, {5, 3}, {5, 4}, {3, 4}};
    AlgoPolygon p5 = {{7, 0}, {7, 1}, {6, 3}};
    AlgoPolygon p6 = {{7, 1}, {7, 3}, {6.5, 2}};
    PolygonList list = {p1, p2, p3, p4, p5, p6};

    auto result = PolygonAlgo::ComposePolygons(list);

    ASSERT_EQ(result.size(), 4);
    ASSERT_TRUE(ContainPolygon(result, {{1, 1}, {2, 1}, {2, 3}, {1, 3}}));
    ASSERT_TRUE(ContainPolygon(result, {{3, 1}, {5, 1}, {5, 4}, {3, 4}}));
    ASSERT_TRUE(ContainPolygon(result, {{3.5, 3}, {4, 2}, {4.5, 3}}));
    ASSERT_TRUE(ContainPolygon(result, {{7, 0}, {7, 3}, {6.5, 2}, {6, 3}}));
}

TEST(TestPolygonAlgp, test_decompose)
{
    AlgoPolygon p1 = {{1, 1}, {2, 1}, {2, 2}, {1, 2}};
    for (auto& item : p1)
        item = item + Vect{0.3, 0.5};
    ASSERT_TRUE(IsConvex(p1));
    auto result = PolygonAlgo::DecomposePolygon(p1);
    ASSERT_EQ(result.size(), 1);
    ASSERT_TRUE(IsConvex(result[0]));

    AlgoPolygon p2 = {{3, 1}, {5, 1}, {5, 3}, {4.5, 3}, {4, 2}, {3.5, 3}, {3, 3}};
    for (auto& item : p1)
        item = item + Vect{0.23, 0.01};
    ASSERT_FALSE(IsConvex(p2));
    result = PolygonAlgo::DecomposePolygon(p2);
    ASSERT_EQ(result.size(), 2);
    ASSERT_TRUE(IsConvex(result[0]));
    ASSERT_TRUE(IsConvex(result[1]));

    AlgoPolygon p3 = {{0, 0}, {2, 0}, {2, 2}, {1.5, 0.5}, {1, 1.5}, {0.5, 1}, {0, 2}};
    for (auto& item : p1)
        item = item + Vect{0.2, 0.11};
    ASSERT_FALSE(IsConvex(p3));
    result = PolygonAlgo::DecomposePolygon(p3);
    ASSERT_EQ(result.size(), 3);
    ASSERT_TRUE(IsConvex(result[0]));
    ASSERT_TRUE(IsConvex(result[1]));
    ASSERT_TRUE(IsConvex(result[2]));
}
