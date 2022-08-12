#include "simulation/particle.h"

#include <gtest/gtest.h>

namespace APowers::UnitTests {

struct ParticleTest : public testing::Test
{
    double epsilon{1e-16};

    Particle standingParticle{3.0, 4.0};

    Particle movingParticle{10.0, 20.0, 0.5, 0.75};

    Particle acceleratingParticle{47.11, 42.0, 1.234, 0.0, 0.0, 0.0815};
};

TEST_F(ParticleTest, ConstructorBehaviorAsExpected)
{
    EXPECT_EQ(standingParticle.position.x, 3.0);
    EXPECT_EQ(standingParticle.position.y, 4.0);
    EXPECT_EQ(standingParticle.velocity.x, 0.0);
    EXPECT_EQ(standingParticle.velocity.y, 0.0);
    EXPECT_EQ(standingParticle.acceleration.x, 0.0);
    EXPECT_EQ(standingParticle.acceleration.y, 0.0);

    EXPECT_EQ(movingParticle.position.x, 10.0);
    EXPECT_EQ(movingParticle.position.y, 20.0);
    EXPECT_EQ(movingParticle.velocity.x, 0.5);
    EXPECT_EQ(movingParticle.velocity.y, 0.75);
    EXPECT_EQ(movingParticle.acceleration.x, 0.0);
    EXPECT_EQ(movingParticle.acceleration.y, 0.0);

    EXPECT_EQ(acceleratingParticle.position.x, 47.11);
    EXPECT_EQ(acceleratingParticle.position.y, 42.0);
    EXPECT_EQ(acceleratingParticle.velocity.x, 1.234);
    EXPECT_EQ(acceleratingParticle.velocity.y, 0.0);
    EXPECT_EQ(acceleratingParticle.acceleration.x, 0.0);
    EXPECT_EQ(acceleratingParticle.acceleration.y, 0.0815);
}

TEST_F(ParticleTest, SimulateOnStandingParticleChangesNothing)
{
    auto const accelerationExpectation = standingParticle.acceleration;
    auto const velocityExpectation     = standingParticle.velocity;
    auto const positionExpectation     = standingParticle.position;

    standingParticle.simulate();

    EXPECT_EQ(standingParticle.acceleration.x, accelerationExpectation.x);
    EXPECT_EQ(standingParticle.acceleration.y, accelerationExpectation.y);
    EXPECT_NEAR(standingParticle.velocity.x, velocityExpectation.x, epsilon);
    EXPECT_NEAR(standingParticle.velocity.y, velocityExpectation.y, epsilon);
    EXPECT_NEAR(standingParticle.position.x, positionExpectation.x, epsilon);
    EXPECT_NEAR(standingParticle.position.y, positionExpectation.y, epsilon);
}

TEST_F(ParticleTest, SimulateOnMovingParticleChangesPositionAsExpected)
{
    auto const accelerationExpectation = movingParticle.acceleration;
    auto const velocityExpectation     = movingParticle.velocity;
    auto const positionExpectation     = movingParticle.position + velocityExpectation;

    movingParticle.simulate();

    EXPECT_EQ(movingParticle.acceleration.x, accelerationExpectation.x);
    EXPECT_EQ(movingParticle.acceleration.y, accelerationExpectation.y);
    EXPECT_NEAR(movingParticle.velocity.x, velocityExpectation.x, epsilon);
    EXPECT_NEAR(movingParticle.velocity.y, velocityExpectation.y, epsilon);
    EXPECT_NEAR(movingParticle.position.x, positionExpectation.x, epsilon);
    EXPECT_NEAR(movingParticle.position.y, positionExpectation.y, epsilon);
}

TEST_F(ParticleTest, SimulateOnAcceleratingParticleChangesParticleStateAsExpected)
{
    auto const accelerationExpectationBefore = acceleratingParticle.acceleration;
    auto const velocityExpectation           = acceleratingParticle.velocity + accelerationExpectationBefore;
    auto const positionExpectation           = acceleratingParticle.position + velocityExpectation;

    acceleratingParticle.simulate();

    EXPECT_NEAR(acceleratingParticle.acceleration.x, (accelerationExpectationBefore.x * 0.8), epsilon);
    EXPECT_NEAR(acceleratingParticle.acceleration.y, (accelerationExpectationBefore.y * 0.8), epsilon);
    EXPECT_NEAR(acceleratingParticle.velocity.x, velocityExpectation.x, epsilon);
    EXPECT_NEAR(acceleratingParticle.velocity.y, velocityExpectation.y, epsilon);
    EXPECT_NEAR(acceleratingParticle.position.x, positionExpectation.x, epsilon);
    EXPECT_NEAR(acceleratingParticle.position.y, positionExpectation.y, epsilon);
}

} // namespace APowers::UnitTests
