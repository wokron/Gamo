#include "physics.h"
#include "algo.h"

namespace gamo
{
    std::vector<b2Shape *> Circle::ToBox2DShape(Vect offset, float rotate, Vect scale)
    {
        _circle.m_radius = _radius * fmin(scale.x, scale.y); // circles cannot be scaled into ellipses
        Matrix m_rotate;
        m_rotate.AsRotate(rotate);
        _circle.m_p = offset + m_rotate * (-Center());
        return {&_circle};
    }

    std::vector<b2Shape *> Polygon::ToBox2DShape(Vect offset, float rotate, Vect scale)
    {
        Matrix m_rotate;
        m_rotate.AsRotate(rotate);
        Matrix m_scale;
        m_scale.AsScale(scale);

        std::vector<Vect> origin_polygon;
        for (int i = 0; i < _vertices.size(); i++)
        {
            Vect point = (m_rotate * (m_scale * (_vertices[i] - Center()))) + offset;
            origin_polygon.push_back(point);
        }

        auto decomposed_polygons = PolygonAlgo::DecomposePolygon(origin_polygon);

        ClearPolygonList();
        for (auto points : decomposed_polygons)
        {
            auto count = points.size();
            b2Vec2 b2vec2_points[count];
            for (int i = 0; i < count; i++)
                b2vec2_points[i] = points[i];
            
            auto shape = new b2PolygonShape();
            shape->Set(b2vec2_points, count);
            _polygons.push_back(shape);
        }

        return _polygons;
    }

    void TerrainPool::Register(Terrain *terrain)
    {
        _count++;
        terrain->SetTerrainPool(this);
    }

    void TerrainPool::Push(std::vector<Vect> polygon)
    {
        if (_count <= 0)
            return;
        _count--;
        _polygons.push_back(polygon);
    }

    std::vector<std::vector<Vect>> TerrainPool::Pull()
    {
        if (_count == 0)
        {
            auto result = PolygonAlgo::ComposePolygons(_polygons);
            _polygons.clear();
            return result;
        }
        return {};
    }

    std::vector<Terrain> Terrain::Union(std::vector<Terrain *> terrains)
    {
        std::vector<std::vector<Vect>> polygons;
        for (auto terrain : terrains)
        {
            polygons.push_back(terrain->Vertices());
        }

        auto composed_terrains = PolygonAlgo::ComposePolygons(polygons);
        std::vector<Terrain> rt;
        for (auto composed_terrain : composed_terrains)
        {
            Terrain t;
            t.Vertices(composed_terrain);
            rt.push_back(t);
        }
        return rt;
    }

    std::vector<b2Shape *> Terrain::ToBox2DShape(Vect offset, float rotate, Vect scale)
    {
        Matrix m_rotate;
        m_rotate.AsRotate(rotate);
        Matrix m_scale;
        m_scale.AsScale(scale);

        std::vector<Vect> result_terrain;
        auto count = Vertices().size();
        for (auto vertice : Vertices())
        {
            auto result = (m_rotate * (m_scale * (vertice - Center()))) + offset;
            result_terrain.push_back(result);
        }

        _terrain_pool->Push(result_terrain);
        auto actual_terrains = _terrain_pool->Pull();

        ClearPolygonList();
        for (auto points : actual_terrains)
        {
            auto count = points.size();
            b2Vec2 b2vec2_points[count];
            for (int i = 0; i < count; i++)
                b2vec2_points[i] = points[i];
            
            auto shape = new b2ChainShape();
            shape->CreateLoop(b2vec2_points, count);
            _polygons.push_back(shape);
        }

        return _polygons;
    }

} // namespace gamo
