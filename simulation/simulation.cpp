#include "simulation.h"

#include <cmath>
#include <limits>

extern "C"
{
#include <noise.h>
}

// because in Visual Studio _USE_MATH_DEFINES does not reach <math.h> through <cmath>
#ifndef M_PI
#include <corecrt_math_defines.h>
#endif

namespace APowers {

Simulation::Simulation(std::uint16_t const cellSize, std::uint16_t const columns, std::uint16_t const rows) noexcept
    : _cellSize{cellSize}, _columns{columns}, _rows{rows}
{
    _cells.resize(static_cast<size_t>(columns) * rows);
}

void Simulation::add(Particle const &particle) noexcept { _particles.push_back(particle); }

size_t Simulation::particleCount() const noexcept { return _particles.size(); }

gsl::span<Particle const> Simulation::particles() const noexcept { return gsl::span<Particle const>{_particles}; }

void Simulation::simulate() noexcept
{
    auto const width  = _cellSize * _columns;
    auto const height = _cellSize * _rows;
    for (auto &p : _particles)
    {
        auto const column = static_cast<uint16_t>(p.position.x) / _cellSize;
        auto const row    = static_cast<uint16_t>(p.position.y) / _cellSize;
        auto const force  = forceDirection(column, row);
        if (force)
        {
            p.acceleration.x = force->x * 0.1;
            p.acceleration.y = force->y * 0.1;
        }
        p.simulate();
        if (p.position.x > width)
        {
            p.position.x -= width;
        }
        else if (p.position.x < 0.0)
        {
            p.position.x += width;
        }

        if (p.position.y > height)
        {
            p.position.y -= height;
        }
        else if (p.position.y < 0.0)
        {
            p.position.y += height;
        }
    }
    ++_step;
}

std::optional<Vector> Simulation::forceDirection(std::uint16_t const column, std::uint16_t const row) noexcept
{
    if (column >= _columns || row >= _rows)
    {
        return {};
    }

    auto &cell = _cells[(static_cast<size_t>(_columns) * row) + column];
    if (cell.first != _step)
    {
        auto const noise     = simplex3(column, row, 0.004F * _step, 6, 0.4F, 3.0F);
        auto const direction = noise * 2.0 * M_PI;
        cell.second.x        = std::sin(direction);
        cell.second.y        = std::cos(direction);
        cell.first           = _step;
    }
    return cell.second;
}

} // namespace APowers
