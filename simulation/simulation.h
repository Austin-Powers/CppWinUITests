#ifndef CPPWINUITESTS_SIMULATION_H
#define CPPWINUITESTS_SIMULATION_H

#include "particle.h"

#include <cstdint>
#include <gsl/gsl>
#include <vector>

namespace APowers {

/// @brief Class for performing a particle simulation on a grid of quadratic cells.
class Simulation final
{
public:
    /// @brief Initializes a new Simulation using the given values.
    ///
    /// @param cellSize The size of a cells in the grid.
    /// @param columns The columns of the grid.
    /// @param rows The rows of the grid.
    Simulation(std::uint16_t cellSize, std::uint16_t columns, std::uint16_t rows) noexcept;

    /// @brief Returns the size of the cells in the grid.
    ///
    /// @return The size of the cells in the grid.
    inline std::uint16_t cellSize() const noexcept { return _cellSize; }

    /// @brief Returns the columns of the grid.
    ///
    /// @return The columns of the grid.
    inline std::uint16_t columns() const noexcept { return _columns; }

    /// @brief Returns the rows of the grid.
    ///
    /// @return The rows of the grid.
    inline std::uint16_t rows() const noexcept { return _rows; }

    /// @brief Adds a particle to the simulation.
    ///
    /// @param particle The particle to add.
    /// @remarks If the position
    void add(Particle const &particle) noexcept;

    /// @brief The particle count of the simulation.
    ///
    /// @return The number of particles in the simulation.
    size_t particleCount() const noexcept;

private:
    /// @brief The size of the cells in the grid.
    std::uint16_t _cellSize{};

    /// @brief The colums of the grid.
    std::uint16_t _columns{};

    /// @brief The rows of the grid.
    std::uint16_t _rows{};

    /// @brief The particles of the simulation.
    std::vector<Particle> _particles{};
};

} // namespace APowers

#endif // !CPPWINUITESTS_SIMULATION_H
