#include "simulation/simulation.h"

#include <cmath>
#include <gtest/gtest.h>
#include <noise.h>

// because in Visual Studio _USE_MATH_DEFINES does not reach <math.h> through <cmath>
#ifndef M_PI
#include <corecrt_math_defines.h>
#endif

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

    Vector calculateForceAt(std::uint16_t const x, std::uint16_t const y) const noexcept
    {
        Vector     result{};
        auto const noise     = simplex2(x, y, 6, 0.4F, 3.0F);
        auto const direction = noise * 2.0 * M_PI;
        result.x             = std::sin(direction);
        result.y             = std::cos(direction);
        return result;
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
    EXPECT_FALSE(simulation.forceDirection(simulation.columns(), simulation.rows()));
    EXPECT_FALSE(simulation.forceDirection(simulation.columns() - 1, simulation.rows()));
    EXPECT_FALSE(simulation.forceDirection(simulation.columns(), simulation.rows() - 1));
}

TEST_F(SimulationTest, ForceDirectionInsideGridAsExepected)
{
    for (auto y = 0U; y < simulation.rows(); ++y)
    {
        for (auto x = 0U; x < simulation.columns(); ++x)
        {
            auto const force = simulation.forceDirection(x, y);
            ASSERT_TRUE(force);
            auto const expectation = calculateForceAt(x, y);
            EXPECT_NEAR(force->x, expectation.x, epsilon);
            EXPECT_NEAR(force->y, expectation.y, epsilon);
        }
    }
}

} // namespace APowers::UnitTests
