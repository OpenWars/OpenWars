#pragma once

namespace OpenWars {
    namespace IO {
        namespace Graphics {
            void init(int width, int height);
            void exit();
            bool shouldClose();
            void beginFrame();
            /**
                Begins attached drawing in camera.
            */
            void beginAttached();
            /**
                Ends attached drawing.
            */
            void endAttached();
            void swapBuffers();
        }
    }
}