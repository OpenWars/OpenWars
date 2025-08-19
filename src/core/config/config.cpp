#include "config.hpp"
#include "../../io/fs/filesystem.hpp"
#include <sstream>
#include <tuple>

OpenWars::Config::Manager::Manager(const std::string& appName)
    : appName(appName) {
    std::string cfgDir = IO::FileSystem::getAppConfigDir(appName);
    configFile = cfgDir + "/config.cfg";
}

bool OpenWars::Config::Manager::load() {
    if(!IO::FileSystem::exists(configFile)) {
        return save();
    }

    std::lock_guard<std::mutex> lk(mutex);

    std::string text = IO::FileSystem::readText(configFile);
    if(text.empty())
        return true;

    std::istringstream iss(text);
    std::string line;
    while(std::getline(iss, line)) {
        line = trim(line);
        if(line.empty() || line[0] == '#')
            continue;
        auto pos = line.find('=');
        if(pos == std::string::npos)
            continue;

        std::string key = trim(line.substr(0, pos));
        std::string val = trim(line.substr(pos + 1));

        // Graphics
        std::apply(
            [&](auto&&... f) {
                (..., (key == f.name ? setField(graphics, f, val) : void()));
            },
            Graphics::fields
        );
        // Player
        std::apply(
            [&](auto&&... f) {
                (..., (key == f.name ? setField(player, f, val) : void()));
            },
            Player::fields
        );
    }

    return true;
}

bool OpenWars::Config::Manager::save() {
    std::lock_guard<std::mutex> lk(mutex);
    std::ostringstream oss;

    std::apply(
        [&](auto&&... f) {
            (..., (oss << f.name << "=" << getFieldValue(graphics, f) << "\n"));
        },
        Graphics::fields
    );
    std::apply(
        [&](auto&&... f) {
            (..., (oss << f.name << "=" << getFieldValue(player, f) << "\n"));
        },
        Player::fields
    );

    return IO::FileSystem::writeText(configFile, oss.str());
}

std::string OpenWars::Config::Manager::trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while(a < b && isspace((unsigned char)s[a]))
        ++a;
    while(b > a && isspace((unsigned char)s[b - 1]))
        --b;
    return s.substr(a, b - a);
}

void OpenWars::Config::Manager::dump() {
    IO::Logging::log("=== Configuration Dump ===");

    IO::Logging::log("%s", "[Graphics]");
    dumpStruct(graphics, Graphics::fields);

    IO::Logging::log("%s", "[Player]");
    dumpStruct(player, Player::fields);

    IO::Logging::log("=== End Configuration Dump ===");
}