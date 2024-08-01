#pragma once

#include <utility>
#include <string>

#include <wheel/singleton.hpp>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace wheel {

#define SDLK_MOUSE_LEFT 0x00001000u
#define SDLK_MOUSE_MIDDLE 0x00001001u
#define SDLK_MOUSE_RIGHT 0x00001002u

class SDL : public Singleton<SDL> {
    friend class Singleton<SDL>;

public:
    static const inline SDL_Color BLACK = {0, 0, 0, 255};
    static const inline SDL_Color WHITE = {255, 255, 255, 255};
    static const inline SDL_Color GRAY = {128, 128, 128, 255};
    static const inline SDL_Color RED = {255, 0, 0, 255};
    static const inline SDL_Color GREEN = {0, 255, 0, 255};
    static const inline SDL_Color BLUE = {0, 0, 255, 255};
    static const inline SDL_Color PINK = {255, 192, 203, 255};

    SDL_Texture* load_img(const std::string& path);
    void render(SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect* dst);
    SDL_Surface* create_surface(int w, int h, SDL_Color color = BLACK, SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface* create_surface(const std::string& text, SDL_Color color = BLACK, int max_w = 0, int max_h = 0);
    SDL_Texture* create_texture(SDL_Surface* surface);
    SDL_Texture* create_texture(const std::string& text, SDL_Color color = BLACK, int max_w = 0, int max_h = 0);
    SDL_Texture* create_texture(int w, int h, SDL_Color color = BLACK, SDL_TextureAccess access = SDL_TEXTUREACCESS_STATIC, SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888);
    void set_color(SDL_Color color);
    void set_target(SDL_Texture* texture);
    void set_alpha_mod(SDL_Texture* texture, Uint8 alpha);
    void set_blend_mode(SDL_Texture* texture, SDL_BlendMode mode);
    void draw_boarder(const SDL_FRect* dst, float border_width, SDL_Color color);
    void draw_rect(const SDL_FRect* dst, SDL_Color color);
    void draw_text(const std::string& text, const SDL_FRect* dst, SDL_Color color, bool mid = false, int max_w = 0, int max_h = 0);
    void draw_text_in_mid(const std::string& text, const SDL_FRect* dst, SDL_Color color);
    std::pair<float, float> get_texture_size(SDL_Texture* texture);
    const char* get_error();
    void destroy(SDL_Texture* texture);
    void destroy(SDL_Surface* surface);
    void clear();
    void present();

    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

private:
    SDL();
    ~SDL();
    SDL(const SDL&) = delete;

    SDL_Window* window_ = nullptr;
    static const int default_font_size_ = 24;
};

inline bool operator==(const SDL_Color& lhs, const SDL_Color& rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

}  // namespace wheel
