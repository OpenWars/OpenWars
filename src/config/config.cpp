#include "config.h"
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>

namespace Config {
char *CONFIG_PATH = nullptr;
void initialize() {
    if (std::getenv("HOME")) {
        strcat(std::getenv("HOME"), "/.config/openwars");
    } else {
        strcat(std::getenv("APPDATA"), "/openwars");
    }
    // TODO: Mac (or does linux implementation fix it?)

    struct stat configPath;
    if (stat(CONFIG_PATH, &configPath) != 0) {
        std::filesystem::create_directories(std::string(CONFIG_PATH) + "/maps");
        std::filesystem::create_directories(std::string(CONFIG_PATH) + "/content");
        std::filesystem::create_directories(std::string(CONFIG_PATH) + "/logs");
    }
    // just assume that it isn't created, it could be a file though
}
} // namespace Config