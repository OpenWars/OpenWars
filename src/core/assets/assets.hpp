#pragma once

namespace OpenWars::Assets {
    class Manager {
      public:
        static Manager& get() {
            static Manager inst;
            return inst;
        }

      protected:
        Manager();                          // Prevent construction
        Manager(const Manager&);            // Prevent construction by copying
        Manager& operator=(const Manager&); // Prevent assignment
        ~Manager();                         // Prevent unwanted destruction
    };
} // namespace OpenWars::Assets