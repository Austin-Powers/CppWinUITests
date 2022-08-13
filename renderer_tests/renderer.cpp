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
    auto const expectedWidth  = simulation.cellSize() * simulation.columns();
    auto const expectedHeight = simulation.cellSize() * simulation.rows();
    auto const imageData      = renderer.image();
    ASSERT_GE(imageData.size(), sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

    BITMAPFILEHEADER fileHeader{};
    BITMAPINFOHEADER infoHeader{};
    std::memcpy(&fileHeader, imageData.data(), sizeof(BITMAPFILEHEADER));
    std::memcpy(&infoHeader, imageData.data() + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
    EXPECT_EQ(fileHeader.bfType, 0x4D42U);
    EXPECT_EQ(fileHeader.bfOffBits, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
    EXPECT_EQ(fileHeader.bfSize, fileHeader.bfOffBits + infoHeader.biSizeImage);
    EXPECT_EQ(infoHeader.biSize, sizeof(BITMAPINFOHEADER));
    EXPECT_EQ(infoHeader.biWidth, expectedWidth);
    EXPECT_EQ(infoHeader.biHeight, expectedHeight);
    EXPECT_EQ(infoHeader.biPlanes, 1U);
    EXPECT_EQ(infoHeader.biBitCount, 32U);
    EXPECT_EQ(infoHeader.biSizeImage, 4U * expectedWidth * expectedHeight);
    EXPECT_EQ(imageData.size(), fileHeader.bfSize);

    auto const pixelData = imageData.subspan(fileHeader.bfOffBits);
    for (auto const b : pixelData)
    {
        ASSERT_EQ(b, 0xFFU);
    }
}

} // namespace APowers::UnitTests
