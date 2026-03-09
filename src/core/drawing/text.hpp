#pragma once

#include "../colors.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <map>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace OpenWars::Drawing {
    extern std::map<int, TTF_Font*> fonts;

    /*
     * Rendering a string via TTF_RenderText_Blended +
     * SDL_CreateTextureFromSurface on every frame is expensive (CPU rasterize →
     * GPU upload every draw call). We cache the resulting SDL_Texture* keyed by
     * (text, fontSize, color) and evict entries that haven't been accessed for
     * TEXTURE_CACHE_MAX_AGE frames.
     */
    namespace TextCache {
        static constexpr int MAX_AGE = 300;     // frames before eviction
        static constexpr size_t MAX_SIZE = 512; // hard cap on entry count

        struct Key {
            std::string text;
            int fontSize;
            uint32_t color; // packed RGBA

            bool operator==(const Key& o) const noexcept {
                return fontSize == o.fontSize && color == o.color &&
                       text == o.text;
            }
        };

        struct KeyHash {
            size_t operator()(const Key& k) const noexcept {
                size_t h = std::hash<std::string>{}(k.text);
                h ^= std::hash<int>{}(k.fontSize) + 0x9e3779b9u + (h << 6) +
                     (h >> 2);
                h ^= std::hash<uint32_t>{}(k.color) + 0x9e3779b9u + (h << 6) +
                     (h >> 2);
                return h;
            }
        };

        struct Entry {
            SDL_Texture* texture = nullptr;
            int w = 0;
            int h = 0;
            uint64_t lastUsed = 0; // frame counter
        };

        using Cache = std::unordered_map<Key, Entry, KeyHash>;

        extern Cache cache;
        extern uint64_t frameCounter;
        void tick();

        SDL_Texture*
        get(const std::string& text,
            int fontSize,
            const Color& color,
            int& w,
            int& h);

        void clear();
    } // namespace TextCache

    TTF_Font* getFont(int size);
    void drawText(const char* text, int x, int y, int fontSize, Color color);
    int measureText(const char* text, int fontSize);
    void drawFPS(int x, int y, int currentFPS);
} // namespace OpenWars::Drawing