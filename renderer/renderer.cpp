#include "renderer.h"

#include <Windows.h>
#include <cstring>

namespace APowers {

Renderer::Renderer(Simulation const &simulation) noexcept
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
}

void Renderer::render() noexcept {}

gsl::span<char const> Renderer::image() const noexcept { return {_imageBuffer}; }

} // namespace APowers
