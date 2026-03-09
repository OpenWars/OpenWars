#include "text.hpp"
#include "../assets/assets.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/log/logging.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Font cache (unchanged)
// ─────────────────────────────────────────────────────────────────────────────

std::map<int, TTF_Font*> OpenWars::Drawing::fonts;

TTF_Font* OpenWars::Drawing::getFont(int size) {
    auto it = fonts.find(size);
    if(it == fonts.end()) {
        TTF_Font* font = OpenWars::Assets::Manager::get().loadFont(
            "fonts/FreeSans.ttf",
            size
        );

        if(!font)
            IO::Logging::error(false, "Failed to load font size %d", size);
        else
            IO::Logging::debug("Successfully loaded font size %d", size);

        fonts[size] = font;
        return font;
    }
    return it->second;
}

OpenWars::Drawing::TextCache::Cache OpenWars::Drawing::TextCache::cache;
uint64_t OpenWars::Drawing::TextCache::frameCounter = 0;

void OpenWars::Drawing::TextCache::tick() {
    ++frameCounter;

    // every 60 frames
    if(frameCounter % 60 != 0)
        return;

    for(auto it = cache.begin(); it != cache.end();) {
        if(frameCounter - it->second.lastUsed > MAX_AGE) {
            SDL_DestroyTexture(it->second.texture);
            it = cache.erase(it);
        } else {
            ++it;
        }
    }
}

SDL_Texture* OpenWars::Drawing::TextCache::get(
    const std::string& text,
    int fontSize,
    const Color& color,
    int& w,
    int& h
) {
    const uint32_t packed = (uint32_t(color.r) << 24) |
                            (uint32_t(color.g) << 16) |
                            (uint32_t(color.b) << 8) | uint32_t(color.a);

    Key key{text, fontSize, packed};

    auto it = cache.find(key);
    if(it != cache.end()) {
        it->second.lastUsed = frameCounter;
        w = it->second.w;
        h = it->second.h;
        return it->second.texture;
    }

    // Cache miss
    TTF_Font* font = OpenWars::Drawing::getFont(fontSize);
    if(!font)
        return nullptr;

    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    SDL_Surface* surface =
        TTF_RenderText_Blended(font, text.c_str(), 0, sdlColor);
    if(!surface)
        return nullptr;

    SDL_Texture* tex =
        SDL_CreateTextureFromSurface(IO::Graphics::getRenderer(), surface);

    int sw = surface->w;
    int sh = surface->h;
    SDL_DestroySurface(surface);

    if(!tex)
        return nullptr;

    if(cache.size() >= MAX_SIZE) {
        auto oldest = cache.begin();
        for(auto jt = cache.begin(); jt != cache.end(); ++jt) {
            if(jt->second.lastUsed < oldest->second.lastUsed)
                oldest = jt;
        }
        SDL_DestroyTexture(oldest->second.texture);
        cache.erase(oldest);
    }

    cache.emplace(key, Entry{tex, sw, sh, frameCounter});

    w = sw;
    h = sh;
    return tex;
}

void OpenWars::Drawing::TextCache::clear() {
    for(auto& [key, entry] : cache)
        SDL_DestroyTexture(entry.texture);
    cache.clear();
}

void OpenWars::Drawing::drawText(
    const char* text,
    int x,
    int y,
    int fontSize,
    Color color
) {
    int w = 0, h = 0;
    SDL_Texture* tex = TextCache::get(text, fontSize, color, w, h);
    if(!tex)
        return;

    SDL_FRect dst = {(float)x, (float)y, (float)w, (float)h};
    SDL_RenderTexture(IO::Graphics::getRenderer(), tex, nullptr, &dst);
}

void OpenWars::Drawing::drawFPS(int x, int y, int currentFPS) {
    // FPS changes every second, so the cached texture stays hot for ~60 frames.
    std::string fpsText = std::to_string(currentFPS) + " FPS";
    drawText(fpsText.c_str(), x, y, 20, Color(0, 255, 0, 255));
}

int OpenWars::Drawing::measureText(const char* text, int fontSize) {
    TTF_Font* font = getFont(fontSize);
    if(!font)
        return 0;

    int w = 0;
    TTF_GetStringSize(font, text, 0, &w, nullptr);
    return w;
}