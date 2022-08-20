#ifndef CPPWINUITEST_RENDERER_RENDERER_H
#define CPPWINUITEST_RENDERER_RENDERER_H

#include "simulation/simulation.h"

#include <cstdint>
#include <gsl/gsl>
#include <vector>

namespace APowers {

/// @brief Class for rendering the simulation to BMP.
class Renderer
{
public:
    /// @brief Initlizes a new renderer using the given simulation as input.
    ///
    /// @param simulation The simulation to render.
    Renderer(Simulation &simulation) noexcept;

    /// @brief Renders the current state of the simulation to the image.
    void render() noexcept;

    /// @brief Returns the result from the last call of render().
    ///
    /// @return Span containing the image data and bitmap header.
    gsl::span<std::uint8_t const> image() const noexcept;

private:
    /// @brief Structure representing a pixel.
    struct Pixel
    {
        /// @brief The blue value of the pixel.
        std::uint8_t blue{};

        /// @brief The green value of the pixel.
        std::uint8_t green{};

        /// @brief The red value of the pixel.
        std::uint8_t red{};

        /// @brief The alpha vlaue of the pixel.
        std::uint8_t alpha{};
    };
    static_assert(sizeof(Pixel) == 4U, "Pixel not 4 bytes");

    /// @brief Reference to the simulation to render.
    Simulation &_simulation;

    /// @brief The buffer containing the image data, including bitmap headers.
    std::vector<std::uint8_t> _imageBuffer{};

    /// @brief Span of the part of the _imageBuffer containing the actual pixel data.
    gsl::span<Pixel> _pixelBuffer{};

    /// @brief The counter for brightening the image.
    uint16_t _brighten{};
};

} // namespace APowers

#endif // !CPPWINUITEST_RENDERER_RENDERER_H
