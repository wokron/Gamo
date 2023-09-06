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

} // namespace gamo
