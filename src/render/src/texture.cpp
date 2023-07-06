#include "render.h"
#include <SDL2/SDL_image.h>

namespace gamo
{
    SDL_Renderer *g_renderer;

    SDL_Window *g_window;    

    Texture *Texture::LoadTexture(std::string texture_path, Color *color_key)
    {
        Assert(g_renderer != nullptr);

        SDL_Surface* source_img = IMG_Load(texture_path.c_str());
        if (source_img == nullptr)
        {
            Log("fail to load image %s, %s", texture_path.c_str(), IMG_GetError());
            return nullptr;
        }

        if (color_key != nullptr)
        {
            SDL_SetColorKey(
                source_img,
                SDL_TRUE,
                SDL_MapRGB(source_img->format, color_key->r, color_key->g, color_key->b)
            );
        }

        SDL_Texture *sdl_texture;
        if ((sdl_texture = SDL_CreateTextureFromSurface(g_renderer, source_img)) == nullptr)
        {
            Log("fail to convert image surface to texture, %s", SDL_GetError());
            SDL_FreeSurface(source_img); // don't forget to free surface
            return nullptr;
        }

        Texture *texture = new Texture(sdl_texture, source_img->w, source_img->w);

        SDL_FreeSurface(source_img); // don't forget to free surface

        return texture;
    }

    Texture::~Texture()
    {
        if (_sdl_texture != nullptr)
        {
            SDL_DestroyTexture(_sdl_texture); // don't forget to free texture
        }
    }

    int Texture::Render(Rect *srcrect, FRect *dstrect, float angle, FPoint *center, FVect *flip)
    {
        Assert(g_renderer != nullptr);
        
        if (_sdl_texture == nullptr)
        {
            Log("_sdl_texture is null, which may indicates that this texture failed to load from image");
            return -1;
        }

        SDL_RendererFlip flip_stategy = SDL_FLIP_NONE;
        if (flip != nullptr)
        {
            if (flip->x < 0)
            {
                flip_stategy = (SDL_RendererFlip)(flip_stategy | SDL_FLIP_HORIZONTAL);
            }
            if (flip->y < 0)
            {
                flip_stategy = (SDL_RendererFlip)(flip_stategy | SDL_FLIP_VERTICAL);
            }
        }

        if (SDL_RenderCopyExF(g_renderer, _sdl_texture, srcrect, dstrect, angle, center, flip_stategy) < 0)
        {
            Log("fail to render texture, %s", SDL_GetError());
            return -1;
        }

        return 0;
    }

    int Texture::SetColorAlpha(ColorAlpha *coloralpha)
    {
        Assert(coloralpha != nullptr);

        int r;

        if (_sdl_texture == nullptr)
        {
            Log("_sdl_texture is null, which may indicates that this texture failed to load from image");
            return -1;
        }

        if ((r = SDL_SetTextureColorMod(_sdl_texture, coloralpha->r, coloralpha->g, coloralpha->b)) < 0)
        {
            Log("fail to set color modulation, %s", SDL_GetError());
            return r;
        }

        if ((r = SDL_SetTextureAlphaMod(_sdl_texture, coloralpha->alpha)) < 0)
        {
            Log("fail to set alpha modulation, %s", SDL_GetError());
            return r;
        }

        return 0;
    }

    SingleSprite *Texture::ClipAndCreateSprite(Rect *cliprect)
    {
        auto *sprite = new SingleSprite(this, cliprect);
        return sprite;
    }
}