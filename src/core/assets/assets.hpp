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

        std::vector<unsigned char> load(const std::string& path) const {
            int32_t err =
                mz_zip_reader_locate_entry(zip_reader, path.c_str(), 0);
            if(err != MZ_OK)
                return {};

            err = mz_zip_reader_entry_open(zip_reader);
            if(err != MZ_OK)
                return {};

            mz_zip_file* fileInfo;
            mz_zip_reader_entry_get_info(zip_reader, &fileInfo);

            int64_t size = fileInfo->uncompressed_size;
            if(size <= 0) {
                mz_zip_reader_entry_close(zip_reader);
                return {};
            }

            std::vector<unsigned char> out(size);
            int32_t read =
                mz_zip_reader_entry_read(zip_reader, out.data(), (int32_t)size);

            mz_zip_reader_entry_close(zip_reader);
            if(read != size)
                return {};

            return out;
        }

        SDL_IOStream* loadRW(const std::string& path) const {
            auto data = load(path);
            if(data.empty())
                return nullptr;

            // SDL copies pointer; we hand ownership to SDL
            void* mem = malloc(data.size());
            memcpy(mem, data.data(), data.size());

            return SDL_IOFromMem(mem, (int)data.size());
        }

        TTF_Font* loadFont(const std::string& path, int ptsize) const {
            SDL_IOStream* rw = loadRW(path);
            if(!rw)
                return nullptr;
            return TTF_OpenFontIO(rw, 1, ptsize);
        }

      protected:
        Manager() {
            zip_reader = mz_zip_reader_create();

            // Open ZIP archive directly from memory
            mem_stream = mz_stream_ioapi_create();
            mz_stream_mem_set_buffer(mem_stream, assets_pak, assets_pak_len);

            mz_zip_reader_open(zip_reader, mem_stream);
        }

        ~Manager() {
            mz_zip_reader_close(zip_reader);
            mz_zip_reader_delete(&zip_reader);

            mz_stream_delete(&mem_stream);
        }

        void* zip_reader = nullptr;
        void* mem_stream = nullptr;

        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;
    };
} // namespace OpenWars::Assets
