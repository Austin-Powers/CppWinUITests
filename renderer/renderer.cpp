#include "renderer.h"

#include <Windows.h>
#include <cstring>
#include <optional>

namespace APowers {

Renderer::Renderer(Simulation &simulation) noexcept : _simulation{simulation}
{
    BITMAPFILEHEADER fileHeader{};
    BITMAPINFOHEADER infoHeader{};
    std::memset(&fileHeader, 0, sizeof(fileHeader));
    std::memset(&infoHeader, 0, sizeof(infoHeader));
    infoHeader.biSize   = sizeof(infoHeader);
    infoHeader.biWidth  = simulation.cellSize() * simulation.columns();
    infoHeader.biHeight = simulation.cellSize() * simulation.rows();
    infoHeader.biPlanes = 1U;
    // we use 32 bit to save some of the hasle with line padding
    infoHeader.biBitCount  = 32U;
    infoHeader.biSizeImage = infoHeader.biWidth * infoHeader.biHeight * (infoHeader.biBitCount / 8U);

    fileHeader.bfType    = 0x4D42U;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fileHeader.bfSize    = fileHeader.bfOffBits + infoHeader.biSizeImage;
    _imageBuffer.resize(fileHeader.bfSize);

    std::memcpy(_imageBuffer.data(), &fileHeader, sizeof(fileHeader));
    std::memcpy(_imageBuffer.data() + sizeof(fileHeader), &infoHeader, sizeof(infoHeader));

    auto const pixelPtr = reinterpret_cast<Pixel *>(_imageBuffer.data() + fileHeader.bfOffBits);
    _pixelBuffer        = {pixelPtr, static_cast<size_t>(infoHeader.biWidth * infoHeader.biHeight)};

    for (auto &pixel : _pixelBuffer)
    {
        pixel.blue  = 0xFFU;
        pixel.green = 0xFFU;
        pixel.red   = 0xFFU;
        pixel.alpha = 0xFFU;
    }
}

void Renderer::render() noexcept
{
    auto const width = _simulation.cellSize() * _simulation.columns();

    auto const pixelPosForParticle = [&](Particle const &p) noexcept -> std::optional<uint32_t> {
        auto const pos = static_cast<uint32_t>(p.position.x) + (static_cast<uint32_t>(p.position.y) * width);
        if (pos >= _pixelBuffer.size())
        {
            return {};
        }
        return pos;
    };

    for (auto const &particle : _simulation.particles())
    {
        auto const pos = pixelPosForParticle(particle);
        if (pos)
        {
            if (_pixelBuffer[*pos].blue >= 0x10)
            {
                _pixelBuffer[*pos].blue -= 0x10;
                _pixelBuffer[*pos].green -= 0x10;
                _pixelBuffer[*pos].red -= 0x10;
            }
        }
    }

    ++_brighten;
    if (_brighten > 63U)
    {
        _brighten = 0U;
        for (auto &pixel : _pixelBuffer)
        {
            if (pixel.blue != 0xFF)
            {
                ++pixel.blue;
                ++pixel.green;
                ++pixel.red;
            }
        }
    }
}

gsl::span<std::uint8_t const> Renderer::image() const noexcept { return {_imageBuffer}; }

} // namespace APowers
