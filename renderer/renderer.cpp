#include "renderer.h"

#include <Windows.h>

namespace APowers {

Renderer::Renderer(Simulation const &simulation) noexcept { _imageBuffer.resize(64U); }

void Renderer::render() noexcept {}

gsl::span<char const> Renderer::image() const noexcept { return {_imageBuffer}; }

} // namespace APowers
