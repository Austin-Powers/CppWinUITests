#include "simulation.h"

#include <noise.h>

namespace APowers {

Simulation::Simulation(std::uint16_t const cellSize, std::uint16_t const columns, std::uint16_t const rows) noexcept
    : _cellSize{cellSize}, _columns{columns}, _rows{rows}
{}

void Simulation::add(Particle const &particle) noexcept { _particles.push_back(particle); }

size_t Simulation::particleCount() const noexcept { return _particles.size(); }

gsl::span<Particle const> Simulation::particles() const noexcept { return gsl::span<Particle const>{_particles}; }

} // namespace APowers
