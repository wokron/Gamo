#include "physics.h"

namespace gamo
{
    b2Shape *Circle::ToBox2DShape(Vect offset, float rotate, Vect scale)
    {
        _circle.m_radius = _radius * fmin(scale.x, scale.y); // circles cannot be scaled into ellipses
        Matrix m_rotate;
        m_rotate.AsRotate(rotate);
        _circle.m_p = offset + m_rotate * (-Center());
        return &_circle;
    }

    b2Shape *Polygon::ToBox2DShape(Vect offset, float rotate, Vect scale)
    {
        Matrix m_rotate;
        m_rotate.AsRotate(rotate * M_PI / 180);
        Matrix m_scale;
        m_scale.AsScale(scale);

        int v_count = _vertices.size();
        b2Vec2 points[v_count];
        for (int i = 0; i < v_count; i++)
        {
            points[i] = (m_rotate * (m_scale * (_vertices[i] - Center()))) + offset;
        }
        _polygon.Set(points, v_count);

        return &_polygon;
    }

} // namespace gamo
