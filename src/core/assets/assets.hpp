#pragma once

#include <SDL3/SDL_iostream.h>
#include <minizip/ioapi.h>
#include <minizip/mz_strm.h>
#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <minizip/mz.h>
#include <minizip/mz_strm_mem.h>
#include <minizip/mz_zip.h>
#include <minizip/mz_zip_rw.h>

extern unsigned char assets_pak[];
extern unsigned int assets_pak_len;

namespace OpenWars::Assets {
    class Manager {
      public:
        static Manager& get() {
            static Manager inst;
            return inst;
        }

        std::vector<unsigned char> load(const std::string& path) const;
        SDL_IOStream* loadRW(const std::string& path) const;
        TTF_Font* loadFont(const std::string& path, int ptsize) const;

      protected:
        Manager();

        ~Manager();

        void* zip_reader = nullptr;
        void* mem_stream = nullptr;

        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;
    };
} // namespace OpenWars::Assets
