#include "particle.h"

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
    acceleration.x *= 0.8;
    acceleration.y *= 0.8;
}

} // namespace APowers
