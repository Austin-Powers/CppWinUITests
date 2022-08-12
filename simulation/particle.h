#ifndef CPPWINUITESTS_SIMULATION_PARTICLE_H
#define CPPWINUITESTS_SIMULATION_PARTICLE_H

#include "vector.h"

#include <cstdint>

namespace APowers {

/// @brief A particle for the particle simulation.
struct Particle final
{
    /// @brief The position of the particle.
    Vector position{};

    /// @brief The velocity of the particle.
    Vector velocity{};

    /// @brief The acceleration of the particle.
    Vector acceleration{};

    /// @brief Initializes a new Particle using the given values.
    ///
    /// @param xPosition The position of the particle on the x-axis.
    /// @param yPosition The position of the particle on the y-axis.
    /// @param xVelocity The velocity of the particle on the x-axis.
    /// @param yVelocity The velocity of the particle on the y-axis.
    /// @param xAcceleration The acceleration of the particle on the x-axis.
    /// @param yAcceleration The acceleration of the particle on the y-axis.
    Particle(double xPosition,
             double yPosition,
             double xVelocity     = 0.0,
             double yVelocity     = 0.0,
             double xAcceleration = 0.0,
             double yAcceleration = 0.0) noexcept;

    /// @brief Initializes a new Particle using the given vectors.
    ///
    /// @param pPosition The position of the particle.
    /// @param pVelocity The velocity of the particle.
    /// @param pAcceleration The acceleration of the particle.
    Particle(Vector const &pPosition, Vector const &pVelocity, Vector const &pAcceleration) noexcept;

    /// @brief Simulates the movement of the particle.
    void simulate() noexcept;
};

} // namespace APowers

#endif // !CPPWINUITESTS_SIMULATION_PARTICLE_H
