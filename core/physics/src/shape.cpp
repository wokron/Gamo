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

        auto count = Vertices().size();
        b2Vec2 vertices[count];

        for (int i = 0; i < count; i++)
        {
            vertices[i] = (m_rotate * (m_scale * (Vertices()[i] - Center()))) + offset;
        }

        _terrain.CreateLoop(vertices, count);
        return {&_terrain};
    }

} // namespace gamo
