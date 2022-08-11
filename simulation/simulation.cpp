#include "simulation.h"

#include <noise.h>

namespace APowers {

Simulation::Simulation(std::uint16_t const cellSize, std::uint16_t const columns, std::uint16_t const rows) noexcept
    : _cellSize{cellSize}, _columns{columns}, _rows{rows}
{}

size_t Simulation::particleCount() const noexcept { return 0U; }

} // namespace APowers
