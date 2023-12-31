#pragma once

#include <vector>
#include "gamo/utils/utils.h"

namespace gamo
{
    class Texture;
    class ColorAlpha;

    class Sprite
    {
    public:
        /// @brief render the sprite to the window
        /// @param position the position of the sprite, or more accurately, the position of the pivot of the sprite. the unit is *Unit*
        /// @param rotate the rotate angle of the sprite, > 0 for clockwise
        /// @param scale the scale size of the sprite, < 0 for flip, null for no scale
        /// @param window_pixel_per_unit relation between window's *Pixel* and *Unit*, this is for camera setting
        /// @return 0 on success, or < 0 on faliure
        /// @return 
        virtual int Render(Vect *position, float rotate, Vect *scale, ColorAlpha *coloralpha, float window_pixel_per_unit) = 0;

        /// @brief the width of the sprite, the unit is *Unit*
        virtual float UnitWidth() = 0;

        /// @brief the height of the sprite, the unit is *Unit*
        virtual float UnitHeight() = 0;

        virtual Vect Pivot() = 0;
    };

    class SingleSprite : public Sprite
    {
    private:
        Texture *_target_texture;
        Rect _sprite_clip;
        Vect _pivot; // the unit of the _pivot is the ratio of the position of the pivot to the length or width of texture
    
    public:
        Rect SpriteClip() { return _sprite_clip; }
        void SpriteClip(Rect sprite_clip) { _sprite_clip = sprite_clip; }
        
        Vect Pivot() { return _pivot; }
        void Pivot(Vect pivot) { _pivot = pivot; }

        float UnitWidth();
        float UnitHeight();
        
        /// @brief create a sprite, setting the pivot to the center of the sprite as default (unit is *Pixel*)
        /// @param target_texture the target texture to clip
        /// @param cliprect the clip area
        SingleSprite(Texture *target_texture, Rect *cliprect);

        int Render(Vect *position, float rotate, Vect *scale, ColorAlpha *coloralpha, float window_pixel_per_unit);
    };

    class AnimateSprite : public Sprite
    {
    private:
        std::vector<SingleSprite*> _sprite_frames = std::vector<SingleSprite*>();
        uint _current_frame = 0;
    public:
        float UnitWidth();
        float UnitHeight();

        Vect Pivot();

        /// @brief add a sprite as a frame
        /// @param sprite the sprite as frame image
        void AddFrame(SingleSprite *sprite);

        /// @brief add sprite as frames, and specify the continue frames
        /// @param continue_frames frame number to continue
        /// @param sprite the sprite as frame image
        void AddFrame(uint continue_frames, SingleSprite *sprite);
        
        int Render(Vect *position, float rotate, Vect *scale, ColorAlpha *coloralpha, float window_pixel_per_unit);
    };
} // namespace gamo
