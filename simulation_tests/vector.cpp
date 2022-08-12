#include "simulation/vector.h"

#include <gtest/gtest.h>

namespace APowers::UnitTests {

struct VectorTest : public testing::Test
{
    double epsilon{1e-16};
    Vector a{3.0, 33.0};
    Vector b{-3.0, 33.3};
    Vector c{-7.0, -42.0};
};

TEST_F(VectorTest, DefaultVectorIsAllZeros)
{
    Vector v{};
    EXPECT_EQ(v.x, 0.0);
    EXPECT_EQ(v.y, 0.0);
}

TEST_F(VectorTest, Construction)
{
    EXPECT_EQ(a.x, 3.0);
    EXPECT_EQ(a.y, 33.0);
    EXPECT_EQ(b.x, -3.0);
    EXPECT_EQ(b.y, 33.3);
    EXPECT_EQ(c.x, -7.0);
    EXPECT_EQ(c.y, -42.0);
}

TEST_F(VectorTest, Addition)
{
    auto r = a + c;
    EXPECT_NEAR(r.x, a.x + c.x, epsilon);
    EXPECT_NEAR(r.y, a.y + c.y, epsilon);
    r += b;
    EXPECT_NEAR(r.x, a.x + b.x + c.x, epsilon);
    EXPECT_NEAR(r.y, a.y + b.y + c.y, epsilon);
}

TEST_F(VectorTest, Subtraction)
{
    auto r = a - c;
    EXPECT_NEAR(r.x, a.x - c.x, epsilon);
    EXPECT_NEAR(r.y, a.y - c.y, epsilon);
    r -= b;
    EXPECT_NEAR(r.x, (a.x - c.x) - b.x, epsilon);
    EXPECT_NEAR(r.y, (a.y - c.y) - b.y, epsilon);
}

} // namespace APowers::UnitTests
