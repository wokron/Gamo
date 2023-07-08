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

    int SingleSprite::Render(FPoint *position, float rotate, FPoint *scale, float window_pixel_per_unit)
    {
        float tppu = _target_texture->PixelPerUnit();
        float wppu = window_pixel_per_unit;

        float sprite_unit_width = _sprite_clip.w / tppu;
        float sprite_unit_height = _sprite_clip.h / tppu;

        float pivot_w = _sprite_clip.w * _pivot.x;
        float pivot_h = _sprite_clip.h * _pivot.y;

        float unit_left_pad = pivot_w / tppu;
        float unit_top_pad = pivot_h / tppu;
        float unit_right_pad = sprite_unit_width - unit_left_pad;
        float unit_bottom_pad = sprite_unit_height - unit_top_pad;

        DoScale(scale, unit_left_pad, unit_top_pad, unit_right_pad, unit_bottom_pad);

        float unit_x = position->x - unit_left_pad;
        float unit_y = position->y - unit_top_pad;
        float unit_w = unit_left_pad + unit_right_pad;
        float unit_h = unit_top_pad + unit_bottom_pad;

        FRect dstrect = {unit_x * wppu, unit_y * wppu, unit_w * wppu, unit_h * wppu};
        FPoint center = {unit_left_pad * wppu, unit_top_pad * wppu};

        int r;
        if ((r = _target_texture->Render(&_sprite_clip, &dstrect, rotate, &center, scale)) < 0)
        {
            return r;
        }

        return 0;
    }

    void inline SingleSprite::DoScale(FPoint *scale, float &unit_left_pad, float &unit_top_pad, float &unit_right_pad, float &unit_bottom_pad)
    {
        if (scale == nullptr)
        {
            return;
        }
        
        if (scale->x > 0)
        {
            unit_left_pad *= scale->x;
            unit_right_pad *= scale->x;
        }
        else // padding reverse when sacle < 0
        {
            float tmp = unit_left_pad; // the value must assign at the same time, so store the previous value of unit_left_pad
            unit_left_pad = unit_right_pad * -scale->x;
            unit_right_pad = tmp * -scale->x;
        }

        if (scale->y > 0)
        {
            unit_top_pad *= scale->y;
            unit_bottom_pad *= scale->y;
        }
        else // padding reverse when sacle < 0
        {
            float tmp = unit_top_pad;
            unit_top_pad = unit_bottom_pad * -scale->y;
            unit_bottom_pad = tmp * -scale->y;
        }
    }

    float AnimateSprite::UnitWidth()
    {
        return _sprite_frames[_current_frame]->UnitWidth();
    }

    float AnimateSprite::UnitHeight()
    {
        return _sprite_frames[_current_frame]->UnitHeight();
    }

    FPoint AnimateSprite::Pivot()
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

    int AnimateSprite::Render(FPoint *position, float rotate, FPoint *scale, float window_pixel_per_unit)
    {
        int r = _sprite_frames[_current_frame]->Render(position, rotate, scale, window_pixel_per_unit);
        _current_frame = (_current_frame + 1) % _sprite_frames.size(); // the animation will loop after reaching the end, just like gif
        return r;
    }

} // namespace gamo
