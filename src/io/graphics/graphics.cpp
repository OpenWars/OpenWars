#include "graphics.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <cmath>
#include <string>

namespace OpenWars::IO::Graphics {
    static SDL_Window* window = nullptr;
    static SDL_Renderer* renderer = nullptr;
    static Camera3D camera;
    static bool shouldCloseFlag = false;
    static std::map<int, TTF_Font*> fonts;
    static uint64_t lastFrameTime = 0;
    static float deltaTime = 0.0f;
    static int frameCount = 0;
    static double lastFPSTime = 0.0;
    static int currentFPS = 0;

    void init(int vsync, bool multisampling) {
        if(!SDL_Init(SDL_INIT_VIDEO)) {
            SDL_Log("SDL init failed: %s", SDL_GetError());
            return;
        }

        if(!TTF_Init()) {
            SDL_Log("TTF init failed: %s", SDL_GetError());
            return;
        }

        uint32_t flags = SDL_WINDOW_RESIZABLE;

        window = SDL_CreateWindow("OpenWars Engine", 1024, 512, flags);

        if(!window) {
            SDL_Log("Window creation failed: %s", SDL_GetError());
            return;
        }

        renderer = SDL_CreateRenderer(window, nullptr);

        if(!renderer) {
            SDL_Log("Renderer creation failed: %s", SDL_GetError());
            return;
        }

        SDL_SetRenderVSync(renderer, vsync);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        camera =
            {Vector3{0, 0, 0}, Vector3{0, 0, 0}, Vector3{0, 1, 0}, 45.0f, 0};

        lastFrameTime = SDL_GetTicksNS();
    }

    void displayDebug(bool debug, bool fps) {
        if(fps) {
            drawFPS(5, 5);
        }
        if(debug) {
            // TODO: Add debug info
        }
    }

    void exit() {
        for(auto& [size, font] : fonts) {
            if(font)
                TTF_CloseFont(font);
        }
        fonts.clear();

        if(renderer)
            SDL_DestroyRenderer(renderer);
        if(window)
            SDL_DestroyWindow(window);

        TTF_Quit();
        SDL_Quit();
    }

    bool shouldClose() {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                shouldCloseFlag = true;
            }
        }
        return shouldCloseFlag;
    }

    void beginFrame() {
        uint64_t currentTime = SDL_GetTicksNS();
        deltaTime = (currentTime - lastFrameTime) / 1000000000.0f;
        lastFrameTime = currentTime;

        frameCount++;
        double currentTimeSeconds = currentTime / 1000000000.0;
        if(currentTimeSeconds - lastFPSTime >= 1.0) {
            currentFPS = frameCount;
            frameCount = 0;
            lastFPSTime = currentTimeSeconds;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void beginAttached() {
        // TODO (sdl3_gpu?)
    }

    void endAttached() {
        // TODO (same as beginAttached)
    }

    Camera3D getCamera() {
        return camera;
    }

    void swapBuffers() {
        SDL_RenderPresent(renderer);
    }

    void clearBackground(Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(renderer);
    }

    void drawRectangle(int x, int y, int width, int height, Color color) {
        SDL_FRect rect = {(float)x, (float)y, (float)width, (float)height};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    void drawRectangleRec(Rectangle rec, Color color) {
        drawRectangle(
            (int)rec.x,
            (int)rec.y,
            (int)rec.width,
            (int)rec.height,
            color
        );
    }

    TTF_Font* getFont(int size) {
        if(fonts.find(size) == fonts.end()) {
            fonts[size] = TTF_OpenFont(
                "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
                size
            );
            /*
            For MacOS?
            if(!fonts[size]) {
                fonts[size] =
                    TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", size);
            }*/
        }
        return fonts[size];
    }

    void drawText(const char* text, int x, int y, int fontSize, Color color) {
        TTF_Font* font = getFont(fontSize);
        if(!font)
            return;

        SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
        SDL_Surface* surface =
            TTF_RenderText_Blended(font, text, sizeof(text), sdlColor);
        if(!surface)
            return;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if(texture) {
            SDL_FRect dstRect =
                {(float)x, (float)y, (float)surface->w, (float)surface->h};
            SDL_RenderTexture(renderer, texture, nullptr, &dstRect);
            SDL_DestroyTexture(texture);
        }

        SDL_DestroySurface(surface);
    }

    void drawFPS(int x, int y) {
        std::string fpsText = std::to_string(currentFPS) + " FPS";
        drawText(fpsText.c_str(), x, y, 20, Color(0, 255, 0, 255));
    }

    void drawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
        SDL_FColor fcolor = {
            color.r / 255.0f,
            color.g / 255.0f,
            color.b / 255.0f,
            color.a / 255.0f
        };

        SDL_Vertex vertices[3] = {
            {{v1.x, v1.y}, fcolor, {0, 0}},
            {{v2.x, v2.y}, fcolor, {0, 0}},
            {{v3.x, v3.y}, fcolor, {0, 0}}
        };

        SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
    }

    void drawLineEx(Vector2 start, Vector2 end, float thickness, Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        if(thickness <= 1.0f) {
            SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
        } else {
            float angle = atan2f(end.y - start.y, end.x - start.x);
            float halfThick = thickness / 2.0f;

            float perpX = -sinf(angle) * halfThick;
            float perpY = cosf(angle) * halfThick;

            SDL_FColor fcolor = {
                color.r / 255.0f,
                color.g / 255.0f,
                color.b / 255.0f,
                color.a / 255.0f
            };

            SDL_Vertex vertices[4] = {
                {{start.x + perpX, start.y + perpY}, fcolor, {0, 0}},
                {{start.x - perpX, start.y - perpY}, fcolor, {0, 0}},
                {{end.x - perpX, end.y - perpY}, fcolor, {0, 0}},
                {{end.x + perpX, end.y + perpY}, fcolor, {0, 0}}
            };

            int indices[6] = {0, 1, 2, 0, 2, 3};
            SDL_RenderGeometry(renderer, nullptr, vertices, 4, indices, 6);
        }
    }

    int measureText(const char* text, int fontSize) {
        TTF_Font* font = getFont(fontSize);
        if(!font)
            return 0;

        int w = 0;
        TTF_GetStringSize(font, text, sizeof(text), &w, nullptr);
        return w;
    }

    Color colorAlpha(Color color, float alpha) {
        color.a = (uint8_t)(alpha * 255.0f);
        return color;
    }

    Color colorBrightness(Color color, float factor) {
        if(factor > 0) {
            color.r = (uint8_t)(color.r + (255 - color.r) * factor);
            color.g = (uint8_t)(color.g + (255 - color.g) * factor);
            color.b = (uint8_t)(color.b + (255 - color.b) * factor);
        } else {
            float absFactor = 1.0f + factor;
            color.r = (uint8_t)(color.r * absFactor);
            color.g = (uint8_t)(color.g * absFactor);
            color.b = (uint8_t)(color.b * absFactor);
        }
        return color;
    }

    bool checkCollisionPointRec(Vector2 point, Rectangle rec) {
        return (
            point.x >= rec.x && point.x <= rec.x + rec.width &&
            point.y >= rec.y && point.y <= rec.y + rec.height
        );
    }

    float sign(Vector2 p1, Vector2 p2, Vector2 p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }

    bool
    checkCollisionPointTriangle(Vector2 p, Vector2 v1, Vector2 v2, Vector2 v3) {
        float d1 = sign(p, v1, v2);
        float d2 = sign(p, v2, v3);
        float d3 = sign(p, v3, v1);

        bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(hasNeg && hasPos);
    }

    int getScreenWidth() {
        int w = 0;
        if(window)
            SDL_GetWindowSize(window, &w, nullptr);
        return w;
    }

    int getScreenHeight() {
        int h = 0;
        if(window)
            SDL_GetWindowSize(window, nullptr, &h);
        return h;
    }

    int getMonitorRefreshRate(int monitor) {
        const SDL_DisplayMode* mode =
            SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
        return mode ? (int)mode->refresh_rate : 60;
    }

    int getCurrentMonitor() {
        return 0; // sdl uses display IDs, simplify for now
    }

    float getFrameTime() {
        return deltaTime;
    }

    double getTime() {
        return SDL_GetTicksNS() / 1000000000.0;
    }

    SDL_Renderer* getRenderer() {
        return renderer;
    }

    SDL_Window* getWindow() {
        return window;
    }
} // namespace OpenWars::IO::Graphics