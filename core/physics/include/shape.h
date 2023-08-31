#pragma once

#include "utils.h"
#include "box2d/box2d.h"

namespace gamo
{
    class Shape
    {
    private:
        Vect _center = {0, 0};

    public:
        Vect Center() { return _center; }
        void Center(Vect center) { _center = center; }

        /// @brief convert Shape to b2SHape type
        /// @param offset offset from rigidbody center to shape center
        /// @param rotate the angle of rotate aroud the center, the unit is radians, > 0 for counterclockwise
        /// @return a b2SHape obj, should not free this pointer
        virtual b2Shape *ToBox2DShape(Vect offset, float rotate) = 0;
    };
    
    class Circle : public Shape
    {
    private:
        float _radius = 1.0f;
        b2CircleShape _circle;

    public:
        float Radius() { return _radius; }
        void Radius(float radius) { _radius = radius; }

        b2Shape *ToBox2DShape(Vect offset, float rotate) override;
    };
    
    class Polygon : public Shape
    {
    private:
        std::vector<Vect> _vertices = {{0.5, 0.5}, {-0.5, 0.5}, {-0.5, -0.5}, {0.5, -0.5}};
        b2PolygonShape _polygon;

    public:
        std::vector<Vect> &Vertices() { return _vertices; }
        void Vertices(std::vector<Vect> vertices) { _vertices = vertices; }
        
        void SetAsBox(float hx, float hy, Vect center)
        {
            _vertices = {{hx, hy}, {-hx, hy}, {-hx, -hy}, {hx, -hy}};
            Center(center);
        }

        b2Shape *ToBox2DShape(Vect offset, float rotate) override;
    };

} // namespace gamo
