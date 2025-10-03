#include "./filesystem.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <SDL3/SDL.h>

std::string OpenWars::IO::FileSystem::readText(const std::string& file) {
    std::ifstream ifs(file);
    if(!ifs.is_open())
        return {};

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

bool OpenWars::IO::FileSystem::writeText(
    const std::string& file,
    const std::string& text
) {
    std::ofstream ofs(file);
    if(!ofs.is_open())
        return false;

    ofs << text;
    return ofs.good();
}

std::vector<unsigned char>
OpenWars::IO::FileSystem::readData(const std::string& file) {
    std::ifstream ifs(file, std::ios::binary);
    if(!ifs.is_open())
        return {};

    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    ifs.read(reinterpret_cast<char*>(buffer.data()), size);

    return buffer;
}

bool OpenWars::IO::FileSystem::writeData(
    const std::string& file,
    const std::vector<unsigned char>& data
) {
    std::ofstream ofs(file, std::ios::binary);
    if(!ofs.is_open())
        return false;

    ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
    return ofs.good();
}

bool OpenWars::IO::FileSystem::exists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool OpenWars::IO::FileSystem::ensureDir(const std::string& path) {
#ifdef _WIN32
    return _mkdir(path.c_str()) == 0 || errno == EEXIST;
#else
    return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
#endif
}

std::string OpenWars::IO::FileSystem::workingDir() {
    char* basePath = (char*)SDL_GetBasePath();
    if(!basePath)
        return "./";

    std::string path(basePath);
    SDL_free(basePath);
    return path;
}

std::string OpenWars::IO::FileSystem::appDir() {
    const char* home = getenv("HOME");
    if(home) {
        return std::string(home) + "/.local/share/";
    }

    char* prefPath = SDL_GetPrefPath("OpenWars", "OpenWars");
    if(!prefPath)
        return "./";

    std::string path(prefPath);
    SDL_free(prefPath);
    return path;
}

std::string
OpenWars::IO::FileSystem::getAppConfigDir(const std::string& appName) {
    std::string dir = appDir() + appName + "/config";
    ensureDir(appDir() + appName);
    ensureDir(dir);
    return dir;
}

std::string
OpenWars::IO::FileSystem::getAppDataDir(const std::string& appName) {
    std::string dir = appDir() + appName + "/data";
    ensureDir(appDir() + appName);
    ensureDir(dir);
    return dir;
}

std::string OpenWars::IO::FileSystem::getLogDir(const std::string& appName) {
    std::string dir = appDir() + appName + "/logs";
    ensureDir(appDir() + appName);
    ensureDir(dir);
    return dir;
}