#pragma once

#include <string>
#include <vector>

namespace OpenWars {
    namespace IO {
        class FileSystem {
          public:
            static std::string readText(const std::string& file);
            static bool
            writeText(const std::string& file, const std::string& text);

            static std::vector<unsigned char> readData(const std::string& file);
            static bool writeData(
                const std::string& file,
                const std::vector<unsigned char>& data
            );

            static bool exists(const std::string& path);

            static bool ensureDir(const std::string& path);

            static std::string workingDir();
            static std::string appDir();

            static std::string getAppConfigDir(const std::string& appName);
            static std::string getAppDataDir(const std::string& appName);
            static std::string getLogDir(const std::string& appName);
        };
    } // namespace IO
} // namespace OpenWars
