#pragma once

#include "utils.h"
#include "box2d/box2d.h"
#include <vector>

namespace gamo
{
    class Shape : public IPrototype<Shape>
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
        virtual std::vector<b2Shape *> ToBox2DShape(Vect offset, float rotate, Vect scale) = 0;

        virtual Shape *Clone() override = 0;
    };
    
    class Circle : public Shape
    {
    private:
        float _radius = 1.0f;
        b2CircleShape _circle;

    public:
        float Radius() { return _radius; }
        void Radius(float radius) { _radius = radius; }

        std::vector<b2Shape *> ToBox2DShape(Vect offset, float rotate, Vect scale) override;

        virtual Circle *Clone() override
        {
            auto obj = new Circle();
            obj->Center(this->Center());
            obj->_radius = _radius;
            return obj;
        }
    };
    
    class Polygon : public Shape
    {
    private:
        std::vector<Vect> _vertices = {{0.5, 0.5}, {-0.5, 0.5}, {-0.5, -0.5}, {0.5, -0.5}};
        std::vector<b2Shape *> _polygons;

    public:
        std::vector<Vect> &Vertices() { return _vertices; }
        void Vertices(std::vector<Vect> vertices) { _vertices = vertices; }
        
        void SetAsBox(float hx, float hy, Vect center)
        {
            _vertices = {{hx, hy}, {-hx, hy}, {-hx, -hy}, {hx, -hy}};
            Center(center);
        }

        std::vector<b2Shape *> ToBox2DShape(Vect offset, float rotate, Vect scale) override;

        virtual Polygon *Clone() override
        {
            auto obj = new Polygon();
            obj->Center(this->Center());
            obj->_vertices = _vertices;
            return obj;
        }

        ~Polygon()
        {
            ClearPolygonList();
        }
    private:
        void ClearPolygonList()
        {
            while (!_polygons.empty())
            {
                auto pre_point = (b2PolygonShape *)_polygons.back();
                _polygons.pop_back();
                delete pre_point;
            }
        }
    };

} // namespace gamo
