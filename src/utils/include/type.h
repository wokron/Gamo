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

        SDL_FPoint ToSDL()
        {
            return {x, y};
        }

        b2Vec2 ToBox2d()
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
        Vect xy;
        Vect wh;

        SDL_FRect ToSDL_F()
        {
            return {xy.x, xy.y, wh.x, wh.y};
        }

        SDL_Rect ToSDL()
        {
            return {(int)roundf(xy.x), (int)roundf(xy.y), (int)roundf(wh.x), (int)roundf(wh.y)};
        }
    };

} // namespace gamo
