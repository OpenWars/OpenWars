#include "core.hpp"

namespace OpenWars {
    static AppInfo currentApp;

    void setApp(const AppInfo& info) {
        currentApp = info;
    }

    const AppInfo& app() {
        return currentApp;
    }
} // namespace OpenWars