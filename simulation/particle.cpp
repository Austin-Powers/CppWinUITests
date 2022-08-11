#include "particle.h"

namespace APowers {

Particle::Particle(double const pXPosition,
                   double const pYPosition,
                   double const pXVelocity,
                   double const pYVelocity,
                   double const pXAcceleration,
                   double const pYAcceleration) noexcept
    : xPosition{pXPosition},
      yPosition{pYPosition},
      xVelocity{pXVelocity},
      yVelocity{pYVelocity},
      xAcceleration{pXAcceleration},
      yAcceleration{pYAcceleration}
{}

void Particle::simulate() noexcept
{
    xVelocity += xAcceleration;
    yVelocity += yAcceleration;
    xPosition += xVelocity;
    yPosition += yVelocity;
    xAcceleration *= 0.8;
    yAcceleration *= 0.8;
}

} // namespace APowers
