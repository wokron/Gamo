#include "render.h"
#include "spdlog/spdlog.h"
#include <SDL2/SDL_image.h>

namespace gamo
{
    Texture *Texture::LoadTexture(std::string texture_path, Color *color_key)
    {
        auto renderer = RenderAsset::GetInstance()->Renderer();
        assert(renderer != nullptr);

        SDL_Surface* source_img = IMG_Load(texture_path.c_str());
        if (source_img == nullptr)
        {
            spdlog::error("fail to load image {}, {}", texture_path, IMG_GetError());
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
        if ((sdl_texture = SDL_CreateTextureFromSurface(renderer, source_img)) == nullptr)
        {
            spdlog::error("fail to convert image surface to texture, {}", SDL_GetError());
            SDL_FreeSurface(source_img); // don't forget to free surface
            return nullptr;
        }

        int r;
        if ((r = SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND)) < 0)
        {
            spdlog::warn("fail to set texture blend mode, {}. this may cause the invalid of transparency when render", SDL_GetError());
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

    int Texture::Render(SDL_Rect *srcrect, SDL_FRect *dstrect, float angle, Vect *center, Vect *flip)
    {
        auto renderer = RenderAsset::GetInstance()->Renderer();
        assert(renderer != nullptr);
        
        if (_sdl_texture == nullptr)
        {
            spdlog::error("_sdl_texture is null, which may indicates that this texture failed to load from image");
            return -1;
        }

        SDL_RendererFlip flip_stategy = SDL_FLIP_NONE;
        SDL_FPoint sdl_center;
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
        if (center != nullptr)
        {
            sdl_center = *center;
        }

        if (SDL_RenderCopyExF(renderer, _sdl_texture, srcrect, dstrect, angle, &sdl_center, flip_stategy) < 0)
        {
            spdlog::error("fail to render texture, {}", SDL_GetError());
            return -1;
        }

        return 0;
    }

    int Texture::SetColorAlpha(ColorAlpha *coloralpha)
    {
        assert(coloralpha != nullptr);

        int r;

        if (_sdl_texture == nullptr)
        {
            spdlog::error("_sdl_texture is null, which may indicates that this texture failed to load from image");
            return -1;
        }

        if ((r = SDL_SetTextureColorMod(_sdl_texture, coloralpha->r, coloralpha->g, coloralpha->b)) < 0)
        {
            spdlog::warn("fail to set color modulation, {}", SDL_GetError());
            return r;
        }

        if ((r = SDL_SetTextureAlphaMod(_sdl_texture, coloralpha->alpha)) < 0)
        {
            spdlog::warn("fail to set alpha modulation, {}", SDL_GetError());
            return r;
        }

        return 0;
    }

    SingleSprite *Texture::ClipAndCreateSprite(SDL_Rect *cliprect)
    {
        auto *sprite = new SingleSprite(this, cliprect);
        return sprite;
    }
}