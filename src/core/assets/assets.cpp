#include "assets.hpp"
#include "../../io/log/logging.hpp"
#include <cstring>
#include <vector>

OpenWars::Assets::Manager::Manager() {
    IO::Logging::debug(
        "assets_pak pointer: %p, length: %u",
        (void*)assets_pak,
        assets_pak_len
    );

    if(assets_pak_len == 0) {
        IO::Logging::error(true, "%s", "Embedded assets are missing or empty!");
        return;
    }

    mem_stream = mz_stream_mem_create();
    if(!mem_stream) {
        IO::Logging::error(true, "%s", "Failed to create memory stream");
        return;
    }

    mz_stream_mem_set_buffer(mem_stream, (void*)assets_pak, assets_pak_len);

    int32_t err = mz_stream_mem_open(mem_stream, NULL, MZ_OPEN_MODE_READ);
    if(err != MZ_OK) {
        IO::Logging::error(true, "Failed to open memory stream: %d", err);
        mz_stream_mem_delete(&mem_stream);
        return;
    }

    zip_reader = mz_zip_reader_create();
    if(!zip_reader) {
        IO::Logging::error(true, "%s", "Failed to create ZIP reader");
        mz_stream_mem_close(mem_stream);
        mz_stream_mem_delete(&mem_stream);
        return;
    }

    err = mz_zip_reader_open(zip_reader, mem_stream);
    if(err != MZ_OK) {
        IO::Logging::error(true, "Failed to open ZIP reader: %d", err);
        mz_zip_reader_delete(&zip_reader);
        mz_stream_mem_close(mem_stream);
        mz_stream_mem_delete(&mem_stream);
    } else {
        IO::Logging::debug("%s", "ZIP reader opened successfully");
    }
}

OpenWars::Assets::Manager::~Manager() {
    if(zip_reader) {
        mz_zip_reader_close(zip_reader);
        mz_zip_reader_delete(&zip_reader);
    }
    if(mem_stream) {
        mz_stream_mem_close(mem_stream);
        mz_stream_mem_delete(&mem_stream);
    }
}

std::vector<unsigned char>
OpenWars::Assets::Manager::load(const std::string& path) const {
    int32_t err = mz_zip_reader_locate_entry(zip_reader, path.c_str(), 0);
    if(err != MZ_OK)
        return {};

    err = mz_zip_reader_entry_open(zip_reader);
    if(err != MZ_OK)
        return {};

    mz_zip_file* fileInfo = nullptr;
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
    if(read != (int32_t)size)
        return {};

    return out;
}

SDL_IOStream* OpenWars::Assets::Manager::loadRW(const std::string& path) const {
    auto data = load(path);
    if(data.empty())
        return nullptr;

    // Move the decompressed bytes onto the heap so their lifetime can be tied
    // to the IOStream instead of this stack frame.
    auto* owned = new std::vector<unsigned char>(std::move(data));

    SDL_IOStream* s = SDL_IOFromMem(owned->data(), (Sint64)owned->size());
    if(!s) {
        delete owned;
        return nullptr;
    }

    // Attach the vector as a property with a cleanup callback.
    // SDL will invoke this callback when SDL_CloseIO(s) is called, ensuring the
    // decompressed buffer is freed at exactly the right moment regardless of
    // which code path closes the stream.
    SDL_SetPointerPropertyWithCleanup(
        SDL_GetIOProperties(s),
        "ow.asset_data",
        owned,
        [](void* /*userdata*/, void* value) {
            delete static_cast<std::vector<unsigned char>*>(value);
        },
        nullptr
    );

    return s;
}

TTF_Font*
OpenWars::Assets::Manager::loadFont(const std::string& path, int ptsize) const {
    SDL_IOStream* rw = loadRW(path);
    if(!rw) {
        IO::Logging::error(
            false,
            "Failed to create IO stream for font: %s",
            path.c_str()
        );
        return nullptr;
    }

    return TTF_OpenFontIO(rw, 1, ptsize);
}