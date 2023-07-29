#pragma once

#include <assert.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <string>
#include "box2d/box2d.h"

namespace gamo
{
    struct Vect
    {
        float x, y;

        static Vect FromVec2(b2Vec2 v)
        {
            return {v.x, v.y};
        }

        Vect operator+(const Vect &b) const
        {
            return {x + b.x, y + b.y};
        }

        Vect operator-(const Vect &b) const
        {
            return {x - b.x, y - b.y};
        }

        Vect operator-() const
        {
            return {-x, -y};
        }

        Vect operator*(const Vect &b) const
        {
            return {x * b.x, y * b.y};
        }

        Vect operator*(float k) const
        {
            return {x * k, y * k};
        }

        Vect operator/(float k) const
        {
            assert(fabs(k) > 1e-6f);
            return {x / k, y / k};
        }

        operator SDL_FPoint()
        {
            return {x, y};
        }

        operator b2Vec2()
        {
            return {x, y};
        }

        std::string ToString()
        {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };

    struct Matrix
    {
        float _11, _12;
        float _21, _22;

        Vect operator*(const Vect &v) const
        {
            return {_11 * v.x + _12 * v.y,
                    _21 * v.x + _22 * v.y};
        }

        Matrix operator*(const Matrix &m) const
        {
            return {
                (_11 * m._11 + _12 * m._21),
                (_11 * m._12 + _12 * m._22),
                (_21 * m._11 + _22 * m._21),
                (_21 * m._12 + _22 * m._22),
            };
        }

        void AsRotate(float theta)
        {
            _11 = cosf(theta), _12 = -sinf(theta);
            _21 = sinf(theta), _22 = cosf(theta);
        }

        void AsScale(const Vect &scale)
        {
            _11 = scale.x, _12 = 0.0f;
            _21 = 0.0f, _22 = scale.y;
        }

        std::string ToString()
        {
            auto line1 = "|" + std::to_string(_11) + ", " + std::to_string(_12) + "|\n";
            auto line2 = "|" + std::to_string(_21) + ", " + std::to_string(_21) + "|";
            return line1 + line2;
        }
    };

    struct Rect
    {
        float x, y, w, h;

        static Rect FromVect(const Vect &xy, const Vect &wh)
        {
            return {xy.x, xy.y, wh.x, wh.y};
        }

        Vect VectXY()
        {
            return {x, y};
        }

        Vect VectWH()
        {
            return {w, h};
        }

        operator SDL_FRect()
        {
            return {x, y, w, h};
        }

        operator SDL_Rect()
        {
            return {(int)roundf(x), (int)roundf(y), (int)roundf(w), (int)roundf(h)};
        }

        std::string ToString()
        {
            auto x_str = "x: " + std::to_string(x) + ", ";
            auto y_str = "y: " + std::to_string(y) + ", ";
            auto w_str = "w: " + std::to_string(w) + ", ";
            auto h_str = "h: " + std::to_string(h);
            return "<" + x_str + y_str + w_str + h_str + ">";
        }
    };

    struct IRect
    {
        int x, y, w, h;

        operator Rect()
        {
            return {(float)x, (float)y, (float)w, (float)h};
        }
    };

} // namespace gamo
