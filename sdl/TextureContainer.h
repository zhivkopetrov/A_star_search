/*
 * TextureContainer.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef SDL_TEXTURECONTAINER_H_
#define SDL_TEXTURECONTAINER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>

//Own components headers

//Forward declarations
struct SDL_Texture;
struct SDL_Surface;
struct SDL_Renderer;
typedef struct _TTF_Font TTF_Font;


class TextureContainer
{
    public:
        TextureContainer();

        virtual ~TextureContainer() = default;

        int32_t init(SDL_Renderer * renderer);

        void deinit();

        void setText(const char *  text,
                     const int32_t fontSize,
                     const uint8_t textureId,
                     int32_t *     outTextWidth,
                     int32_t *     outTextHeight);

        inline SDL_Texture * getTexture(const uint8_t textureId) const
        {
            return _textures[textureId];
        }

        inline SDL_Rect getTextureFrameRect(const uint8_t textureId,
                                            const uint8_t frame)
        {
            return _textureFrameRects[textureId][frame];
        }

    private:
        int32_t loadTextures();

        int32_t loadSingleTexture(const char *  filePath,
                                  const uint8_t textureId);

        /** @brief used to create SDL_Texture from provided SDL_Surface
         *         NOTE: if SDL_Texture is successful - the input SDL_Surface
         *                       is not longer needed -> therefore it is freed.
         *
         *  @param SDL_Surface *& - input SDL_Surface
         *  @param SDL_Texture *& - dynamically created SDL_Texture
         *
         *  @returns int32_t      - error code
         * */
        int32_t loadTextureFromSurface(SDL_Surface *& surface,
                                       SDL_Texture *& outTexture);

        void populateTextureFrameRects();


        //the textures we'll be drawing
        std::vector<SDL_Texture *>         _textures;

        //individual texture source frame rectangles
        std::vector<std::vector<SDL_Rect>> _textureFrameRects;

        //the global fonts we'll be using
        TTF_Font *                         _fontSmall;
        TTF_Font *                         _fontBig;

        //The Hardware Accelerated Renderer
        SDL_Renderer *                     _renderer;

        //the color we'll be using for the text
        SDL_Color                          _color;
};

#endif /* SDL_TEXTURECONTAINER_H_ */


