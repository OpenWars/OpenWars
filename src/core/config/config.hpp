#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "../../io/log/logging.hpp"

namespace OpenWars::Config {
    template <typename Owner, typename T> struct Field {
        const char* name;
        T Owner::* member;
    };

    struct Graphics {
        bool multisampling = true;
        int vsync = 1;
        bool showFps = true;
        bool displayDebugInfo = false;

        static inline auto fields = std::make_tuple(
            Field<Graphics, bool>{"multisampling", &Graphics::multisampling},
            Field<Graphics, int>{"vsync", &Graphics::vsync},
            Field<Graphics, bool>{"showFps", &Graphics::showFps},
            Field<Graphics, bool>{
                "displayDebugInfo",
                &Graphics::displayDebugInfo
            }
        );
    };

    class Manager {
        public:
        Manager(const std::string& appName);
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;

        /*
         * Register any struct that exposes a static `fields` tuple of Field<>.
         * Must be called before init(). The reference must outlive this
         * Manager.
         */
        template <typename Struct> void registerSection(Struct& s) {
            sections.push_back(
                {[this, &s]() {
                     std::apply(
                         [&](auto&&... f) { (..., applyField(s, f)); },
                         Struct::fields
                     );
                 },
                 [this, &s](std::ostringstream& oss) {
                     std::apply(
                         [&](auto&&... f) {
                             (...,
                              (oss << f.name << "=" << fieldValue(s, f)
                                   << "\n"));
                         },
                         Struct::fields
                     );
                 },
                 [this, &s]() {
                     std::apply(
                         [&](auto&&... f) {
                             (...,
                              IO::Logging::debug(
                                  "  %s = %s",
                                  f.name,
                                  fieldValue(s, f).c_str()
                              ));
                         },
                         Struct::fields
                     );
                 }}
            );
        }

        void init();
        bool load();
        bool save();
        void dump();

        Graphics graphics;

        private:
        struct Section {
            std::function<void()> apply;
            std::function<void(std::ostringstream&)> serialize;
            std::function<void()> dump;
        };

        std::string configFile;
        std::map<std::string, std::string> data;
        std::vector<Section> sections;
        std::mutex mutex;

        std::string trim(const std::string& s);

        template <typename Struct, typename T>
        void applyField(Struct& s, const Field<Struct, T>& f) {
            auto it = data.find(f.name);
            if(it == data.end())
                return;
            const std::string& val = it->second;
            if constexpr(std::is_same_v<T, bool>)
                s.*(f.member) = (val == "1" || val == "true");
            else if constexpr(std::is_same_v<T, int>)
                s.*(f.member) = std::stoi(val);
            else if constexpr(std::is_same_v<T, std::string>)
                s.*(f.member) = val;
        }

        template <typename Struct, typename T>
        std::string
        fieldValue(const Struct& s, const Field<Struct, T>& f) const {
            if constexpr(std::is_same_v<T, bool>)
                return s.*(f.member) ? "1" : "0";
            else if constexpr(std::is_same_v<T, int>)
                return std::to_string(s.*(f.member));
            else if constexpr(std::is_same_v<T, std::string>)
                return s.*(f.member);
            return {};
        }
    };
} // namespace OpenWars::Config