#include "io/graphics/graphics.hpp"
#include "io/log/logging.hpp"

using namespace OpenWars;

int main(){
    IO::Logging::init();
    IO::Graphics::init(512, 512);

    while (!IO::Graphics::shouldClose()){
        IO::Graphics::beginFrame();
        IO::Graphics::swapBuffers();
    }
    IO::Graphics::exit();
}