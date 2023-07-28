#include "render.h"

namespace gamo
{
    SingleSprite::SingleSprite(Texture *target_texture, Rect *cliprect)
    {
        _target_texture = target_texture;
        _sprite_clip = *cliprect;
        _pivot = {0.5f, 0.5f}; // the default pivot position is the center of sprite
    }

    float SingleSprite::UnitWidth()
    {
        float tppu = _target_texture->PixelPerUnit();
        return _sprite_clip.w / tppu;
    }

    float SingleSprite::UnitHeight()
    {
        float tppu = _target_texture->PixelPerUnit();
        return _sprite_clip.h / tppu;
    }

    int SingleSprite::Render(Vect *position, float rotate, Vect *scale, ColorAlpha *coloralpha, float window_pixel_per_unit)
    {
        float tppu = _target_texture->PixelPerUnit();
        float wppu = window_pixel_per_unit;

        Vect pivot_wh = _sprite_clip.VectWH() * _pivot;
        Vect sprite_unit_wh = _sprite_clip.VectWH() / tppu;

        Vect unit_left_top = -pivot_wh / tppu;
        Vect unit_right_bottom = sprite_unit_wh + unit_left_top;

        Matrix scale_m;
        scale != nullptr ? scale_m.AsScale(*scale) : scale_m.AsScale({1, 1});

        unit_left_top = scale_m * unit_left_top;
        unit_right_bottom = scale_m * unit_right_bottom;

        if (unit_left_top.x > unit_right_bottom.x)
            std::swap(unit_left_top.x, unit_right_bottom.x);
        if (unit_left_top.y > unit_right_bottom.y)
            std::swap(unit_left_top.y, unit_right_bottom.y);

        Vect unit_xy = *position + unit_left_top;
        Vect unit_wh = unit_right_bottom - unit_left_top;

        Vect window_xy = unit_xy * wppu;
        Vect window_wh = unit_wh * wppu;

        Rect dstrect = Rect::FromVect(window_xy, window_wh);
        Vect center = -unit_left_top * wppu;

        int r;
        if ((r = _target_texture->SetColorAlpha(coloralpha)) < 0)
        {
            return r;
        }

        if ((r = _target_texture->Render(&_sprite_clip, &dstrect, rotate, &center, scale)) < 0)
        {
            return r;
        }

        return 0;
    }

    float AnimateSprite::UnitWidth()
    {
        return _sprite_frames[_current_frame]->UnitWidth();
    }

    float AnimateSprite::UnitHeight()
    {
        return _sprite_frames[_current_frame]->UnitHeight();
    }

    Vect AnimateSprite::Pivot()
    {
        return _sprite_frames[_current_frame]->Pivot();
    }

    void AnimateSprite::AddFrame(SingleSprite *sprite)
    {
        _sprite_frames.push_back(sprite);
    }

    void AnimateSprite::AddFrame(uint continue_frames, SingleSprite *sprite)
    {
        for (int i = 0; i < continue_frames; i++)
        {
            AddFrame(sprite);
        }
    }

    int AnimateSprite::Render(Vect *position, float rotate, Vect *scale, ColorAlpha *coloralpha, float window_pixel_per_unit)
    {
        int r = _sprite_frames[_current_frame]->Render(position, rotate, scale, coloralpha, window_pixel_per_unit);
        _current_frame = (_current_frame + 1) % _sprite_frames.size(); // the animation will loop after reaching the end, just like gif
        return r;
    }

} // namespace gamo
