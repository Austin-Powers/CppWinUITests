#ifndef CPPWINUITESTS_PARTICLE_H
#define CPPWINUITESTS_PARTICLE_H

#include <cstdint>

namespace APowers {

/// @brief A particle for the particle simulation.
struct Particle final
{
    /// @brief The position of the particle on the x-axis.
    double xPosition{};

    /// @brief The position of the particle on the y-axis.
    double yPosition{};

    /// @brief The velocity of the particle on the x-axis.
    double xVelocity{};

    /// @brief The velocity of the particle on the y-axis.
    double yVelocity{};

    /// @brief The acceleration of the particle on the x-axis.
    double xAcceleration{};

    /// @brief The acceleration of the particle on the y-axis.
    double yAcceleration{};

    /// @brief Initializes a new Particle using the given values.
    ///
    /// @param pXPosition The position of the particle on the x-axis.
    /// @param pYPosition The position of the particle on the y-axis.
    /// @param pXVelocity The velocity of the particle on the x-axis.
    /// @param pYVelocity The velocity of the particle on the y-axis.
    /// @param pXAcceleration The acceleration of the particle on the x-axis.
    /// @param pYAcceleration The acceleration of the particle on the y-axis.
    Particle(double pXPosition,
             double pYPosition,
             double pXVelocity     = 0.0,
             double pYVelocity     = 0.0,
             double pXAcceleration = 0.0,
             double pYAcceleration = 0.0) noexcept;
};

} // namespace APowers

#endif // !CPPWINUITESTS_PARTICLE_H
