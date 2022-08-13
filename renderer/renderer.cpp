#include "renderer.h"

#include <Windows.h>
#include <cstring>

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
    auto const          cellSize = _simulation.cellSize();
    std::uint16_t const width    = cellSize * _simulation.columns();
    std::uint16_t const height   = cellSize * _simulation.rows();

    for (auto y = 0U; y < height; ++y)
    {
        for (auto x = 0U; x < width; ++x)
        {
            auto const force = _simulation.forceDirection(x / cellSize, y / cellSize);
            if (force)
            {
                auto &pixel = _pixelBuffer[(y * width) + x];
                pixel.blue  = static_cast<std::uint8_t>(std::abs(force->x) * 255.0);
                pixel.green = static_cast<std::uint8_t>(std::abs(force->y) * 255.0);
            }
        }
    }
}

gsl::span<std::uint8_t const> Renderer::image() const noexcept { return {_imageBuffer}; }

} // namespace APowers
