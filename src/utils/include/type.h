#pragma once

#include <assert.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "box2d/box2d.h"

namespace gamo
{
    struct Vect
    {
        float x, y;

        Vect operator+(const Vect &b)
        {
            return {x + b.x, y + b.y};
        }

        Vect operator-(const Vect &b)
        {
            return {x - b.x, y - b.y};
        }

        Vect operator-()
        {
            return {-x, -y};
        }

        Vect operator*(const Vect &b)
        {
            return {x * b.x, y * b.y};
        }

        Vect operator*(float k)
        {
            return {x * k, y * k};
        }

        Vect operator/(float k)
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
    };

    struct Matrix
    {
        float _11, _12;
        float _21, _22;

        Vect operator*(const Vect &v)
        {
            return {_11 * v.x + _12 * v.y,
                    _21 * v.x + _22 * v.y};
        }

        Matrix operator*(const Matrix &m)
        {
            return {
                (_11 * m._11 + _12 * m._21),
                (_11 * m._12 + _11 * m._22),
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
