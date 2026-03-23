#include "config.hpp"
#include "../../io/fs/filesystem.hpp"
#include <sstream>

namespace OpenWars::Config {
    Manager::Manager(const std::string& appName)
        : configFile(IO::FileSystem::getAppConfigDir(appName) + "/config.cfg") {
        registerSection(graphics);
    }

    void Manager::init() {
        if(!load())
            save();
        dump();
    }

    bool Manager::load() {
        if(!IO::FileSystem::exists(configFile))
            return false;

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
            data[trim(line.substr(0, pos))] = trim(line.substr(pos + 1));
        }

        for(auto& s : sections)
            s.apply();

        return true;
    }

    bool Manager::save() {
        std::lock_guard<std::mutex> lk(mutex);
        std::ostringstream oss;

        for(auto& s : sections)
            s.serialize(oss);

        return IO::FileSystem::writeText(configFile, oss.str());
    }

    void Manager::dump() {
        IO::Logging::debug("%s", "=== Config ===");
        for(auto& s : sections)
            s.dump();
        IO::Logging::debug("%s", "==============");
    }

    std::string Manager::trim(const std::string& s) {
        size_t a = 0, b = s.size();
        while(a < b && isspace((unsigned char)s[a]))
            ++a;
        while(b > a && isspace((unsigned char)s[b - 1]))
            --b;
        return s.substr(a, b - a);
    }
} // namespace OpenWars::Config