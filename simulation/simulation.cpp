#include "simulation.h"

#include <cmath>
#include <limits>
#include <noise.h>

// because in Visual Studio _USE_MATH_DEFINES does not reach <math.h> through <cmath>
#ifndef M_PI
#include <corecrt_math_defines.h>
#endif

namespace APowers {

Simulation::Simulation(std::uint16_t const cellSize, std::uint16_t const columns, std::uint16_t const rows) noexcept
    : _cellSize{cellSize}, _columns{columns}, _rows{rows}
{}

void Simulation::add(Particle const &particle) noexcept { _particles.push_back(particle); }

size_t Simulation::particleCount() const noexcept { return _particles.size(); }

gsl::span<Particle const> Simulation::particles() const noexcept { return gsl::span<Particle const>{_particles}; }

void Simulation::simulate() noexcept {}

std::optional<Vector> Simulation::forceDirection(std::uint16_t const column, std::uint16_t const row) noexcept
{
    if (column >= _columns || row >= _rows)
    {
        return {};
    }
    auto const noise     = simplex2(column, row, 6, 0.4F, 3.0F);
    auto const direction = noise * 2.0 * M_PI;
    return Vector{std::sin(direction), std::cos(direction)};
}

} // namespace APowers
