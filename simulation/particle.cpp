#include "particle.h"

#include <cmath>

namespace APowers {

Particle::Particle(double const xPosition,
                   double const yPosition,
                   double const xVelocity,
                   double const yVelocity,
                   double const xAcceleration,
                   double const yAcceleration) noexcept
    : position{xPosition, yPosition}, velocity{xVelocity, yVelocity}, acceleration{xAcceleration, yAcceleration}
{}

Particle::Particle(Vector const &pPosition, Vector const &pVelocity, Vector const &pAcceleration) noexcept
    : position{pPosition}, velocity{pVelocity}, acceleration{pAcceleration}
{}

void Particle::simulate() noexcept
{
    velocity += acceleration;
    position += velocity;

    // limit velocity
    if (velocity.lengthSquared() > 4.0)
    {
        velocity.x *= 0.75;
        velocity.y *= 0.75;
    }
}

} // namespace APowers
