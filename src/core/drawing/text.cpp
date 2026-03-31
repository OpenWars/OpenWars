#include "text.hpp"
#include "../assets/assets.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/log/logging.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>

std::map<int, TTF_Font*> OpenWars::Drawing::fonts;

TTF_Font* OpenWars::Drawing::getFont(int size) {
    auto it = fonts.find(size);
    if(it == fonts.end()) {
        TTF_Font* font = OpenWars::Assets::Manager::get().loadFont(
            "fonts/RethinkSans.ttf",
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

    // Only scan for stale entries every 60 frames to keep tick() cheap.
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

// TTF_GetStringSize() re-runs the shaper on every call.  UI code (carousel
// item bounds, button layout) calls measureText in tight loops, so we cache
// the width result keyed by (text, fontSize).  The cache grows at most as fast
// as the number of distinct (string, size) pairs ever seen — in practice a few
// dozen entries.
namespace {
    struct MeasureKey {
        std::string text;
        int fontSize;

        bool operator==(const MeasureKey& o) const noexcept {
            return fontSize == o.fontSize && text == o.text;
        }
    };

    struct MeasureKeyHash {
        size_t operator()(const MeasureKey& k) const noexcept {
            size_t h = std::hash<std::string>{}(k.text);
            h ^= std::hash<int>{}(k.fontSize) + 0x9e3779b9u + (h << 6) +
                 (h >> 2);
            return h;
        }
    };

    std::unordered_map<MeasureKey, int, MeasureKeyHash> measureCache;
} // anonymous namespace

int OpenWars::Drawing::measureText(const char* text, int fontSize) {
    MeasureKey key{text, fontSize};

    auto it = measureCache.find(key);
    if(it != measureCache.end())
        return it->second;

    TTF_Font* font = getFont(fontSize);
    if(!font)
        return 0;

    int w = 0;
    TTF_GetStringSize(font, text, 0, &w, nullptr);
    measureCache.emplace(std::move(key), w);
    return w;
}