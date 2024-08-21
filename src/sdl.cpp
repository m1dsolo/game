#include <game/sdl.hpp>

#include <filesystem>
#include <cmath>

#include <wheel/log.hpp>

namespace wheel {

SDL::SDL() {
    Log::assert_(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) == 0, "SDL init failed");

    // no cursor
    SDL_HideCursor();

    // img
    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    Log::assert_(IMG_Init(img_flags) == img_flags, "SDL_image init failed");

    // audio
    int mix_flags = MIX_INIT_MP3;
    Log::assert_(Mix_Init(mix_flags) == mix_flags, "SDL_mixer init failed");

    SDL_AudioSpec audio_spec = {MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, MIX_DEFAULT_FREQUENCY};
    Mix_OpenAudio(0, &audio_spec);

    // ttf
    Log::assert_(TTF_Init() == 0, "SDL_ttf init failed");
    const std::string& font_path = std::filesystem::current_path().string() + "/resources/font/SauceCodeProNerdFont-Medium.ttf";
    font = TTF_OpenFont(font_path.c_str(), default_font_size_);

    // window and renderer
    // window = SDL_CreateWindow("My Simple Game", config_resource.w, config_resource.h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    window_ = SDL_CreateWindow("My Simple Game", 1920, 1080, SDL_WINDOW_OPENGL);
    Log::assert_(window_, SDL_GetError);
    renderer = SDL_CreateRenderer(window_, nullptr);
    Log::assert_(renderer, SDL_GetError);
}

SDL::~SDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_);

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* SDL::load_img(const std::string& path) {
    return IMG_LoadTexture(renderer, path.c_str());
}

void SDL::render(SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect* dst) {
    SDL_RenderTexture(renderer, texture, src, dst);
}

SDL_Surface* SDL::create_surface(int w, int h, SDL_Color color, SDL_PixelFormat format) {
    SDL_Surface* surface = SDL_CreateSurface(w, h, format);
    if (color != BLACK) {
        SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, color.r, color.g, color.b, color.a));
    }
    return surface;
}

SDL_Surface* SDL::create_surface(const std::string& text, SDL_Color color, int max_w, int max_h, int wrap_len) {
    if (wrap_len) {
        return TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, wrap_len);
    } else {
        if (max_w) {
            int w, h;
            TTF_SizeText(font, text.c_str(), &w, &h);
            float scale = std::max((float)w / max_w, (float)h / max_h);
            if (scale > 1.) {
                TTF_SetFontSize(font, int(default_font_size_ / scale));
            }
        }
        auto surface = TTF_RenderText_Blended(font, text.c_str(), color);
        TTF_SetFontSize(font, default_font_size_);
        return surface;
    }
}

SDL_Texture* SDL::create_texture(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(renderer, surface);
}

SDL_Texture* SDL::create_texture(const std::string& text, SDL_Color color, int max_w, int max_h, int wrap_len) {
    SDL_Surface* surface = create_surface(text, color, max_w, max_h, wrap_len);
    SDL_Texture* texture = create_texture(surface);
    destroy(surface);
    return texture;
}

SDL_Texture* SDL::create_texture(int w, int h, SDL_Color color, SDL_TextureAccess access, SDL_PixelFormat format) {
    if (color == BLACK) {
        return SDL_CreateTexture(renderer, format, access, w, h);
    }

    if (access != SDL_TEXTUREACCESS_STATIC) {
        Log::assert_(false, "create_texture: access must be SDL_TEXTUREACCESS_STATIC");
        return nullptr;
    }

    SDL_Surface* surface = create_surface(w, h, color, format);
    SDL_Texture* texture = create_texture(surface);
    destroy(surface);
    return texture;
}

SDL_Texture* SDL::create_filled_circle_texture(int radius, SDL_Color color) {
    auto texture = create_texture(radius * 2, radius * 2, BLACK, SDL_TEXTUREACCESS_TARGET);
    set_target(texture);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) < (radius * radius)) {
                draw_point(radius + dx, radius + dy, color);
            }
        }
    }
    set_target(nullptr);

    return texture;
}

SDL_Texture* SDL::create_border_circle_texture(int radius, SDL_Color color) {
    auto texture = create_texture(radius * 2 + 1, radius * 2 + 1, BLACK, SDL_TEXTUREACCESS_TARGET);
    const int diameter = (radius * 2);

    int x = (radius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    set_target(texture);
    int cx = radius;
    int cy = radius;
    while (x >= y) {
        draw_point(cx - x, cy - y, color);
        draw_point(cx - x, cy + y, color);
        draw_point(cx + x, cy - y, color);
        draw_point(cx + x, cy + y, color);
        draw_point(cx - y, cy - x, color);
        draw_point(cx - y, cy + x, color);
        draw_point(cx + y, cy - x, color);
        draw_point(cx + y, cy + x, color);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
    set_target(nullptr);

    return texture;
}

SDL_Texture* SDL::merge_textures(SDL_Texture* texture1, SDL_Texture* texture2) {
    auto [w, h] = get_texture_size(texture1);
    auto texture = create_texture(w, h, BLACK, SDL_TEXTUREACCESS_TARGET);
    set_target(texture);
    render(texture1, nullptr, nullptr);
    render(texture2, nullptr, nullptr);
    set_target(nullptr);
    return texture;
}

void SDL::set_color(SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void SDL::set_blend_mode(SDL_Surface* surface, SDL_BlendMode mode) {
    SDL_SetSurfaceBlendMode(surface, mode);
}

void SDL::set_blend_mode(SDL_Texture* texture, SDL_BlendMode mode) {
    SDL_SetTextureBlendMode(texture, mode);
}

void SDL::set_target(SDL_Texture* texture) {
    SDL_SetRenderTarget(renderer, texture);
}

void SDL::set_alpha_mod(SDL_Texture* texture, Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture, alpha);
}

void SDL::draw_border(const SDL_FRect* dst, float border_w, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    auto [x, y, w, h] = *dst;

    SDL_FRect topRect = {x, y, w, border_w};
    SDL_RenderFillRect(renderer, &topRect);

    SDL_FRect leftRect = {x, y, border_w, h};
    SDL_RenderFillRect(renderer, &leftRect);

    SDL_FRect bottomRect = {x, y + h - border_w, w, border_w};
    SDL_RenderFillRect(renderer, &bottomRect);

    SDL_FRect rightRect = {x + w - border_w, y, border_w, h};
    SDL_RenderFillRect(renderer, &rightRect);
}

void SDL::draw_point(int x, int y, SDL_Color color) {
    set_color(color);
    SDL_RenderPoint(renderer, x, y);
}

void SDL::draw_rect(const SDL_FRect* dst, SDL_Color color) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, dst);
}

// dst is left top corner
void SDL::draw_text(const std::string& text, const SDL_FRect* dst, SDL_Color color, bool mid, int max_w, int max_h, int wrap_len) {
    SDL_Texture* texture = create_texture(text, color, max_w, max_h, wrap_len);
    auto [w, h] = get_texture_size(texture);
    SDL_FRect text_dst = *dst;
    if (mid) {
        text_dst.x += (dst->w - w) / 2;
        text_dst.y += (dst->h - h) / 2;
    }
    text_dst.w = w;
    text_dst.h = h;
    render(texture, nullptr, &text_dst);
    destroy(texture);
}

void SDL::blit(SDL_Surface* src_surface, const SDL_FRect* src, SDL_Surface* dst_surface, const SDL_FRect* dst) {
    SDL_BlitSurface(src_surface, (SDL_Rect*)src, dst_surface, (SDL_Rect*)dst);
}

std::pair<float, float> SDL::get_texture_size(SDL_Texture* texture) {
    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    return {w, h};
}

const char* SDL::get_error() {
    return SDL_GetError();
}

void SDL::destroy(SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
}

void SDL::destroy(SDL_Surface* surface) {
    SDL_DestroySurface(surface);
}

void SDL::clear() {
    SDL_RenderClear(renderer);
}

void SDL::present() {
    SDL_RenderPresent(renderer);
}

}  // namespace wheel
