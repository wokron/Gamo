#pragma once

#include <string>
#include <SDL2/SDL.h>
#include "utils.h"

namespace gamo
{
    struct Color
    {
        u_char r;
        u_char g;
        u_char b;
    };

    struct ColorAlpha
    {
        u_char r;
        u_char g;
        u_char b;
        u_char alpha;
    };

    class SingleSprite;

    class Texture
    {
    private:
        SDL_Texture *_sdl_texture = nullptr;
        float _pixel_per_unit = 100.0;
        int _pixel_width = 0;
        int _pixel_height = 0;

    public:
        float PixelPerUnit() { return _pixel_per_unit; }
        void PixelPerUnit(float pixel_per_unit) { _pixel_per_unit = pixel_per_unit; }

        int PixelWidth() { return _pixel_width; }
        
        int PixelHeight() { return _pixel_height; }

        /// @brief create texture from the given path, and set the color key to give a transparent background.
        /// Post-condition: the sdl_image and the global renderer must be initialized
        /// @param texture_path the path of the texture, which must be a picture file with format png, jpg or webp.
        /// @param color_key the color for background, null for no color key
        static Texture *LoadTexture(std::string texture_path, Color *color_key);

        Texture(SDL_Texture *sdl_texture, int pixel_width, int pixel_height)
            : _sdl_texture(sdl_texture), _pixel_width(pixel_width), _pixel_height(pixel_height) { }

        ~Texture();

        /// @brief render the window use part of the texture
        /// Post-condition: the global renderer must be initialized
        /// @param srcrect the rect area in texture to rendered to window, the unit is *Pixel*
        /// @param dstrect the rect area in window to render, the unit is *Pixel*
        /// @param angle An angle in degrees that indicates the rotation that will be applied to dstrect
        /// @param center A pointer to a point indicating the point around which dstrect will be rotated
        /// (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2), the unit is *Pixel*
        /// @param flip A pointer to a vector indicating the flip position 
        /// @return 0 on success and -1 on error
        int Render(Rect *srcrect, FRect *dstrect, float angle, FPoint *center, FVect *flip);

        /// @brief set the rgb and alpha values of the texture, which can change the color and the transparency of the texture in render
        /// Post-condition: coloralpha is not null
        /// @param coloralpha the color and alpha value to set
        /// @return 0 on success or < 0 on faliure.
        int SetColorAlpha(ColorAlpha *coloralpha);

        /// @brief clip the texture to create a sprite. 
        /// this function will not check if the area to clip is on the area of the texture, 
        /// or if the texture is actually loaded.
        /// @param cliprect the rect area to clip
        /// @return the sprite created
        SingleSprite *ClipAndCreateSprite(Rect *cliprect);
    };
} // namespace gamo
