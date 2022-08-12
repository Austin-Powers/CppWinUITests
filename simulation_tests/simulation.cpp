#include "simulation/simulation.h"

#include <gtest/gtest.h>

namespace APowers::UnitTests {

struct SimulationTest : public testing::Test
{
    double     epsilon{1e-16};
    Particle   particle0{20.0, 30.0};
    Particle   particle1{25.0, 40.0};
    Simulation simulation{40U, 16U, 9U};

    void compareParticles(Particle const &reality, Particle const &expectation) const noexcept
    {
        EXPECT_NEAR(reality.position.x, expectation.position.x, epsilon);
        EXPECT_NEAR(reality.position.y, expectation.position.y, epsilon);
        EXPECT_NEAR(reality.velocity.x, expectation.velocity.x, epsilon);
        EXPECT_NEAR(reality.velocity.y, expectation.velocity.y, epsilon);
        EXPECT_NEAR(reality.acceleration.x, expectation.acceleration.x, epsilon);
        EXPECT_NEAR(reality.acceleration.y, expectation.acceleration.y, epsilon);
    }
};

TEST_F(SimulationTest, NewlyConstructedInstanceStateAsExpected)
{
    EXPECT_EQ(simulation.cellSize(), 40U);
    EXPECT_EQ(simulation.columns(), 16U);
    EXPECT_EQ(simulation.rows(), 9U);
    EXPECT_EQ(simulation.particleCount(), 0U);

    Simulation const simulation2{256U, 4U, 3U};
    EXPECT_EQ(simulation2.cellSize(), 256U);
    EXPECT_EQ(simulation2.columns(), 4U);
    EXPECT_EQ(simulation2.rows(), 3U);
    EXPECT_EQ(simulation2.particleCount(), 0U);
}

TEST_F(SimulationTest, AddingParticlesIncreasesParticleCount)
{
    simulation.add(particle0);
    EXPECT_EQ(simulation.particleCount(), 1U);
    simulation.add(particle1);
    EXPECT_EQ(simulation.particleCount(), 2U);
    simulation.add(particle0);
    EXPECT_EQ(simulation.particleCount(), 3U);
}

TEST_F(SimulationTest, ParticlesAreReturnedCorrectlyByTheSimulation)
{
    simulation.add(particle0);
    simulation.add(particle1);
    simulation.add(particle0);
    auto const particles = simulation.particles();
    EXPECT_EQ(particles.size(), 3U);
    compareParticles(particles[0], particle0);
    compareParticles(particles[1], particle1);
    compareParticles(particles[2], particle0);
}

TEST_F(SimulationTest, ForceDirectionReturnsEmptyOptionalIfOutOfGrid)
{
    EXPECT_FALSE(simulation.forceDirection(16U, 9U));
    EXPECT_FALSE(simulation.forceDirection(16U, 8U));
    EXPECT_FALSE(simulation.forceDirection(15U, 9U));
}

} // namespace APowers::UnitTests
