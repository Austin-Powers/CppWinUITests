#include "renderer/renderer.h"

#include "simulation/simulation.h"

#include <Windows.h>
#include <cstring>
#include <gtest/gtest.h>

namespace APowers::UnitTests {

struct RendererTests : public testing::Test
{
    Simulation simulation{20U, 16U, 9U};

    Renderer renderer{simulation};
};

TEST_F(RendererTests, RendererInitializedCorrectly)
{
    auto const imageData = renderer.image();
    ASSERT_GE(imageData.size(), sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
}

} // namespace APowers::UnitTests
