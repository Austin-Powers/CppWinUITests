#include "renderer/renderer.h"
#include "simulation/simulation.h"

#include <fstream>

namespace APowers {

int realMain(int argc, char **argv) noexcept
{
    Simulation simulation{40U, 16U, 9U};
    Renderer   renderer{simulation};
    renderer.render();

    std::ofstream imageFile("test.bmp", std::ios::binary);
    if (imageFile.is_open())
    {
        auto const imageData = renderer.image();
        imageFile.write(reinterpret_cast<char const *>(imageData.data()), imageData.size());
    }
    return 0;
}

} // namespace APowers

int main(int argc, char **argv) noexcept { return APowers::realMain(argc, argv); }
