#pragma once

#include <string>
#include <mutex>
#include <tuple>

namespace OpenWars {
    namespace Config {
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
                Field<Graphics, bool>{
                    "multisampling",
                    &Graphics::multisampling
                },
                Field<Graphics, int>{"vsync", &Graphics::vsync},
                Field<Graphics, bool>{"showFps", &Graphics::showFps},
                Field<Graphics, bool>{
                    "displayDebugInfo",
                    &Graphics::displayDebugInfo
                }
            );
        };

        struct Player {
            std::string name = "Player";

            static inline auto fields = std::make_tuple(
                Field<Player, std::string>{"playerName", &Player::name}
            );
        };

        class Manager {
          public:
            Manager(const std::string& appName = "OpenWars");

            bool load();
            bool save();

            Graphics graphics;
            Player player;

          private:
            std::string configFile;
            std::string appName;
            std::mutex mutex;

            std::string trim(const std::string& s);

            template <typename Struct, typename T>
            void setField(
                Struct& s,
                const Field<Struct, T>& f,
                const std::string& val
            ) {
                if constexpr(std::is_same_v<T, bool>)
                    s.*(f.member) = (val == "1" || val == "true");
                else if constexpr(std::is_same_v<T, int>)
                    s.*(f.member) = std::stoi(val);
                else if constexpr(std::is_same_v<T, std::string>)
                    s.*(f.member) = val;
            }

            template <typename Struct, typename T>
            std::string
            getFieldValue(const Struct& s, const Field<Struct, T>& f) {
                if constexpr(std::is_same_v<T, bool>)
                    return s.*(f.member) ? "1" : "0";
                else if constexpr(std::is_same_v<T, int>)
                    return std::to_string(s.*(f.member));
                else if constexpr(std::is_same_v<T, std::string>)
                    return s.*(f.member);
            }
        };
    } // namespace Config
} // namespace OpenWars
