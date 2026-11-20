#include "rive_thorvg.h"
#include <array>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <memory>
#include <vector>

int main(int argc, char** argv)
{
    if (argc != 2) return 1;
    std::ifstream stream(argv[1], std::ios::binary);
    const std::vector<uint8_t> file{std::istreambuf_iterator<char>(stream), {}};
    if (file.empty() || stream.bad()) return 1;

    std::array<uint32_t, 160 * 120> pixels{};
    rive_thorvg_init(0);
    std::unique_ptr<rive_instance_t, decltype(&rive_destroy)> scene(
        rive_create(pixels.data(), 160, 120, RIVE_PIXEL_FORMAT_ARGB8888), rive_destroy);
    bool moved = false;
    if (scene && rive_load(scene.get(), file.data(), file.size()) && rive_advance(scene.get(), 0)) {
        const auto first = pixels;
        for (int tick = 0; tick < 30; ++tick) rive_advance(scene.get(), 1.0f / 60);
        moved = pixels != first;
    }
    scene.reset();
    rive_thorvg_term();
    return moved ? 0 : 1;
}
