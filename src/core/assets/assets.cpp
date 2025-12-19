#include "assets.hpp"

#include "../../io/log/logging.hpp"

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

    // Create memory stream for minizip-ng
    mem_stream = mz_stream_mem_create();
    if(!mem_stream) {
        IO::Logging::error(true, "%s", "Failed to create memory stream");
        return;
    }

    // Set the buffer
    mz_stream_mem_set_buffer(mem_stream, (void*)assets_pak, assets_pak_len);

    // Open the memory stream
    int32_t err = mz_stream_mem_open(mem_stream, NULL, MZ_OPEN_MODE_READ);
    if(err != MZ_OK) {
        IO::Logging::error(true, "Failed to open memory stream: %d", err);
        mz_stream_mem_delete(&mem_stream);
        return;
    }

    // Create ZIP reader
    zip_reader = mz_zip_reader_create();
    if(!zip_reader) {
        IO::Logging::error(true, "%s", "Failed to create ZIP reader");
        mz_stream_mem_close(mem_stream);
        mz_stream_mem_delete(&mem_stream);
        return;
    }

    // Open ZIP reader with the memory stream
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

SDL_IOStream* OpenWars::Assets::Manager::loadRW(const std::string& path) const {
    auto data = load(path);
    if(data.empty())
        return nullptr;

    // SDL copies pointer; we hand ownership to SDL
    void* mem = malloc(data.size());
    memcpy(mem, data.data(), data.size());

    return SDL_IOFromMem(mem, (int)data.size());
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