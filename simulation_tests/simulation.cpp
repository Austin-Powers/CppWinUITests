#include "simulation/simulation.h"

#include <gtest/gtest.h>

namespace APowers::UnitTests {

struct SimulationTest : public testing::Test
{
    Simulation simulation{40U, 16U, 9U};
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

} // namespace APowers::UnitTests
