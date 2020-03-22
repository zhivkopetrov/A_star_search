#ifndef THIRDPARTY_SDLUTILS_TEXTURE_H_
#define THIRDPARTY_SDLUTILS_TEXTURE_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations
class Color;
class Rectangle;
struct SDL_Rect;
struct SDL_Point;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;
struct DrawParams;
struct SDL_Color;
typedef struct _TTF_Font TTF_Font;

class Texture
{
    public:
        //forbid the default constructor and destructor
        Texture() = delete;
        virtual ~Texture() = delete;

        //forbid the copy and move constructors
        Texture(const Texture & other) = delete;
        Texture(Texture && other) = delete;

        //forbid the copy and move assignment operators
        Texture & operator=(const Texture & other) = delete;
        Texture & operator=(Texture && other) = delete;

        /** @brief used to free SDL_Surface
         *
         *  @param SDL_Surface *& the surface to be freed
         * */
        static void freeSurface(SDL_Surface *& surface);

        /** @brief used to free SDL_Texture
         *
         *  @param SDL_Texture*& the texture to be freed
         * */
        static void freeTexture(SDL_Texture *& texture);

        /** @brief used to load SDL_Surface from file on the hard drive
         *
         *  @param const char *   - absolute path to file
         *  @param SDL_Surface *& - dynamically created SDL_Surface
         *
         *  @returns int32_t      - error code
         * */
        static int32_t loadSurfaceFromFile(const char *   path,
                                           SDL_Surface *& outSurface);

        /** @brief used to create SDL_Texture from provided SDL_Surface
         *         NOTE: if SDL_Texture is successful - the input SDL_Surface
         *                       is not longer needed -> therefore it is freed.
         *
         *  @param SDL_Surface *& - input SDL_Surface
         *  @param SDL_Texture *& - dynamically created SDL_Texture
         *
         *  @returns int32_t     - error code
         * */
        static int32_t loadTextureFromSurface(SDL_Surface *& surface,
                                              SDL_Texture *& outTexture);

        /** @brief used to load SDL_Texture from provided user text
         *
         *  @param const char *   - user provided text
         *  @param TTF_Font *     - SDL_Font that is used
         *  @param const SDL_Color &  - color used to create the text
         *  @param SDL_Texture *& - dynamically created SDL_Surface
         *  @param int32_t *      - width of the created SDL_Surface text
         *  @param int32_t *      - height of the created SDL_Surface text
         *
         *  @returns int32_t      - error code
         * */
        static int32_t loadFromText(const char *text,
                                    TTF_Font *font,
                                    const SDL_Color &color,
                                    SDL_Texture *& outTexture,
                                    int32_t *outTextWidth,
                                    int32_t *outTextHeight);

        /** @brief used to acquire renderer pointer that will be performing
          *                                         the graphical render calls.
          *
          *  @param SDL_Renderer * - the actual hardware renderer
          * */
        static void setRenderer(SDL_Renderer * renderer);

    private:
        /** Keep pointer to the actual renderer,
          * since Texture class function will be used all the time
          * and in order not to pass pointers on every single function call
          * */
        static SDL_Renderer * _renderer;
};

#endif /* THIRDPARTY_SDLUTILS_TEXTURE_H_ */


