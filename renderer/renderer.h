#ifndef CPPWINUITEST_RENDERER_RENDERER_H
#define CPPWINUITEST_RENDERER_RENDERER_H

#include "simulation/simulation.h"

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
    Renderer(Simulation const &simulation) noexcept;

    /// @brief Renders the current state of the simulation to the image.
    void render() noexcept;

    /// @brief Returns the result from the last call of render().
    ///
    /// @return Span containing the image data and bitmap header.
    gsl::span<char const> image() const noexcept;

private:
    /// @brief The buffer containing the image data, including bitmap headers.
    std::vector<char> _imageBuffer{};
};

} // namespace APowers

#endif // !CPPWINUITEST_RENDERER_RENDERER_H
