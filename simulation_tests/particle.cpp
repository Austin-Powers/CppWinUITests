#include "simulation/particle.h"

#include <gtest/gtest.h>

namespace APowers::UnitTests {

struct ParticleTests : public testing::Test
{
    Particle particle{47.11, 42.0, 1.234, 0.0, 0.0, 0.0815};
};

TEST_F(ParticleTests, ConstructorBehaviorAsExpected)
{
    EXPECT_EQ(particle.xPosition, 47.11);
    EXPECT_EQ(particle.yPosition, 42.0);
    EXPECT_EQ(particle.xVelocity, 1.234);
    EXPECT_EQ(particle.yVelocity, 0.0);
    EXPECT_EQ(particle.xAcceleration, 0.0);
    EXPECT_EQ(particle.yAcceleration, 0.0815);

    Particle defaultParticle{3.0, 4.0};
    EXPECT_EQ(defaultParticle.xPosition, 3.0);
    EXPECT_EQ(defaultParticle.yPosition, 4.0);
    EXPECT_EQ(defaultParticle.xVelocity, 0.0);
    EXPECT_EQ(defaultParticle.yVelocity, 0.0);
    EXPECT_EQ(defaultParticle.xAcceleration, 0.0);
    EXPECT_EQ(defaultParticle.yAcceleration, 0.0);
}

} // namespace APowers::UnitTests
