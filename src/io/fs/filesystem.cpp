#include "./filesystem.hpp"

namespace raylib {
#include "raylib.h"
}

std::string OpenWars::IO::FileSystem::readText(const std::string& file) {
    char* txt = raylib::LoadFileText(file.c_str());
    if(!txt)
        return {};
    std::string out(txt);
    raylib::UnloadFileText(txt);
    return out;
}

bool OpenWars::IO::FileSystem::writeText(
    const std::string& file,
    const std::string& text
) {
    return raylib::SaveFileText(file.c_str(), const_cast<char*>(text.c_str()));
}

std::vector<unsigned char>
OpenWars::IO::FileSystem::readData(const std::string& file) {
    int size = 0;
    unsigned char* ptr = raylib::LoadFileData(file.c_str(), &size);
    if(!ptr)
        return {};
    std::vector<unsigned char> buf(ptr, ptr + size);
    raylib::UnloadFileData(ptr);
    return buf;
}

bool OpenWars::IO::FileSystem::writeData(
    const std::string& file,
    const std::vector<unsigned char>& data
) {
    return raylib::SaveFileData(
        file.c_str(),
        (void*)data.data(),
        (int)data.size()
    );
}

bool OpenWars::IO::FileSystem::exists(const std::string& path) {
    return raylib::FileExists(path.c_str());
}

bool OpenWars::IO::FileSystem::ensureDir(const std::string& path) {
    return raylib::MakeDirectory(path.c_str()) == 0;
}

std::string OpenWars::IO::FileSystem::workingDir() {
    return raylib::GetWorkingDirectory();
}

std::string OpenWars::IO::FileSystem::appDir() {
    return raylib::GetApplicationDirectory();
}

std::string
OpenWars::IO::FileSystem::getAppConfigDir(const std::string& appName) {
    std::string dir = appDir() + appName + "/config";
    ensureDir(dir);
    return dir;
}

std::string
OpenWars::IO::FileSystem::getAppDataDir(const std::string& appName) {
    std::string dir = appDir() + appName + "/data";
    ensureDir(dir);
    return dir;
}

std::string OpenWars::IO::FileSystem::getLogDir(const std::string& appName) {
    std::string dir = appDir() + appName + "/logs";
    ensureDir(dir);
    return dir;
}