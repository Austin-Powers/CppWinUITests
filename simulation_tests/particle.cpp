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
    EXPECT_EQ(standingParticle.xPosition, 3.0);
    EXPECT_EQ(standingParticle.yPosition, 4.0);
    EXPECT_EQ(standingParticle.xVelocity, 0.0);
    EXPECT_EQ(standingParticle.yVelocity, 0.0);
    EXPECT_EQ(standingParticle.xAcceleration, 0.0);
    EXPECT_EQ(standingParticle.yAcceleration, 0.0);

    EXPECT_EQ(movingParticle.xPosition, 10.0);
    EXPECT_EQ(movingParticle.yPosition, 20.0);
    EXPECT_EQ(movingParticle.xVelocity, 0.5);
    EXPECT_EQ(movingParticle.yVelocity, 0.75);
    EXPECT_EQ(movingParticle.xAcceleration, 0.0);
    EXPECT_EQ(movingParticle.yAcceleration, 0.0);

    EXPECT_EQ(acceleratingParticle.xPosition, 47.11);
    EXPECT_EQ(acceleratingParticle.yPosition, 42.0);
    EXPECT_EQ(acceleratingParticle.xVelocity, 1.234);
    EXPECT_EQ(acceleratingParticle.yVelocity, 0.0);
    EXPECT_EQ(acceleratingParticle.xAcceleration, 0.0);
    EXPECT_EQ(acceleratingParticle.yAcceleration, 0.0815);
}

TEST_F(ParticleTest, SimulateOnStandingParticleChangesNothing)
{
    auto const xAccelerationExpectation = standingParticle.xAcceleration;
    auto const yAccelerationExpectation = standingParticle.yAcceleration;
    auto const xVelocityExpectation     = standingParticle.xVelocity;
    auto const yVelocityExpectation     = standingParticle.yVelocity;
    auto const xPositionExpectation     = standingParticle.xPosition;
    auto const yPositionExpectation     = standingParticle.yPosition;

    standingParticle.simulate();

    EXPECT_EQ(standingParticle.xAcceleration, xAccelerationExpectation);
    EXPECT_EQ(standingParticle.yAcceleration, yAccelerationExpectation);
    EXPECT_NEAR(standingParticle.xVelocity, xVelocityExpectation, epsilon);
    EXPECT_NEAR(standingParticle.yVelocity, yVelocityExpectation, epsilon);
    EXPECT_NEAR(standingParticle.xPosition, xPositionExpectation, epsilon);
    EXPECT_NEAR(standingParticle.yPosition, yPositionExpectation, epsilon);
}

TEST_F(ParticleTest, SimulateOnMovingParticleChangesPositionAsExpected)
{
    auto const xAccelerationExpectation = movingParticle.xAcceleration;
    auto const yAccelerationExpectation = movingParticle.yAcceleration;
    auto const xVelocityExpectation     = movingParticle.xVelocity;
    auto const yVelocityExpectation     = movingParticle.yVelocity;
    auto const xPositionExpectation     = movingParticle.xPosition + xVelocityExpectation;
    auto const yPositionExpectation     = movingParticle.yPosition + yVelocityExpectation;

    movingParticle.simulate();

    EXPECT_EQ(movingParticle.xAcceleration, xAccelerationExpectation);
    EXPECT_EQ(movingParticle.yAcceleration, yAccelerationExpectation);
    EXPECT_NEAR(movingParticle.xVelocity, xVelocityExpectation, epsilon);
    EXPECT_NEAR(movingParticle.yVelocity, yVelocityExpectation, epsilon);
    EXPECT_NEAR(movingParticle.xPosition, xPositionExpectation, epsilon);
    EXPECT_NEAR(movingParticle.yPosition, yPositionExpectation, epsilon);
}

TEST_F(ParticleTest, SimulateOnAcceleratingParticleChangesParticleStateAsExpected)
{
    auto const xAccelerationExpectationBefore = acceleratingParticle.xAcceleration;
    auto const yAccelerationExpectationBefore = acceleratingParticle.yAcceleration;
    auto const xVelocityExpectation           = acceleratingParticle.xVelocity + xAccelerationExpectationBefore;
    auto const yVelocityExpectation           = acceleratingParticle.yVelocity + yAccelerationExpectationBefore;
    auto const xPositionExpectation           = acceleratingParticle.xPosition + xVelocityExpectation;
    auto const yPositionExpectation           = acceleratingParticle.yPosition + yVelocityExpectation;

    acceleratingParticle.simulate();

    EXPECT_NEAR(acceleratingParticle.xAcceleration, (xAccelerationExpectationBefore * 0.8), epsilon);
    EXPECT_NEAR(acceleratingParticle.yAcceleration, (yAccelerationExpectationBefore * 0.8), epsilon);
    EXPECT_NEAR(acceleratingParticle.xVelocity, xVelocityExpectation, epsilon);
    EXPECT_NEAR(acceleratingParticle.yVelocity, yVelocityExpectation, epsilon);
    EXPECT_NEAR(acceleratingParticle.xPosition, xPositionExpectation, epsilon);
    EXPECT_NEAR(acceleratingParticle.yPosition, yPositionExpectation, epsilon);
}

} // namespace APowers::UnitTests
