#pragma once

namespace OpenWars {
    struct AppInfo {
        const char* name = "OpenWars";
        const char* version = "0.0.1";
        bool testingBuild = false;
    };

    void setApp(const AppInfo& info);
    const AppInfo& app();
} // namespace OpenWars