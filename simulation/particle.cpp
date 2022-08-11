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
} // namespace APowers
