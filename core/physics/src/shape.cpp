#include "physics.h"

namespace gamo
{
    b2Shape *Circle::ToBox2DShape(Vect offset, float rotate)
    {
        _circle.m_radius = _radius;
        Matrix m_rotate;
        m_rotate.AsRotate(rotate);
        _circle.m_p = offset + m_rotate * (-Center());
        return &_circle;
    }

    b2Shape *Polygon::ToBox2DShape(Vect offset, float rotate)
    {
        Matrix m_rotate;
        m_rotate.AsRotate(rotate);

        int v_count = _vertices.size();
        b2Vec2 points[v_count];
        for (int i = 0; i < v_count; i++)
        {
            points[i] = (m_rotate * (_vertices[i] - Center())) + offset;
        }
        _polygon.Set(points, v_count);

        return &_polygon;
    }

} // namespace gamo
