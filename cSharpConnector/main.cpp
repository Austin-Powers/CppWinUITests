#include "renderer/renderer.h"
#include "simpleSocket.h"
#include "simulation/simulation.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>

namespace APowers {

int realMain(int argc, char **argv) noexcept
{
    if (argc < 5)
    {
        std::cerr << "Too few arguments\n";
        std::cerr << "[Port][CellSize][Columns][Rows]\n";
        return 1;
    }

    auto const parseArgument = [&](std::uint32_t index) noexcept -> std::optional<std::uint32_t> {
        try
        {
            return std::stoul(argv[index]);
        }
        catch (std::invalid_argument const &)
        {
            return {};
        }
        catch (std::out_of_range const &)
        {
            return {};
        }
    };

    auto const port = parseArgument(1U);
    if (!port)
    {
        std::cerr << "Unable to parse Port\n";
        std::cerr << "[Port][CellSize][Columns][Rows]\n";
        return 1;
    }
    auto const cellSize = parseArgument(2U);
    if (!cellSize)
    {
        std::cerr << "Unable to parse CellSize\n";
        std::cerr << "[Port][CellSize][Columns][Rows]\n";
        return 1;
    }
    auto const columns = parseArgument(3U);
    if (!columns)
    {
        std::cerr << "Unable to parse Columns\n";
        std::cerr << "[Port][CellSize][Columns][Rows]\n";
        return 1;
    }
    auto const rows = parseArgument(4U);
    if (!rows)
    {
        std::cerr << "Unable to parse Rows\n";
        std::cerr << "[Port][CellSize][Columns][Rows]\n";
        return 1;
    }

    Simulation simulation{
        static_cast<uint16_t>(*cellSize), static_cast<uint16_t>(*columns), static_cast<uint16_t>(*rows)};
    Renderer     renderer{simulation};
    SimpleSocket socket{static_cast<uint16_t>(*port)};

    std::array<std::uint8_t, 256U> receiveBuffer{};
    socket.connect();

    auto const checkForMessage =
        [](gsl::span<std::uint8_t const> data, char const *const message, size_t n) noexcept -> bool {
        return std::strncmp(reinterpret_cast<char const *>(data.data()), message, n) == 0;
    };
    while (socket.isConnected())
    {
        auto const bytesReceived = socket.receiveData(receiveBuffer, std::chrono::milliseconds{100U});
        if (bytesReceived.empty())
        {
            continue;
        }
        if (checkForMessage(bytesReceived, "add", 3))
        {
            try
            {
                size_t     index{};
                auto const message = reinterpret_cast<char const *>(bytesReceived.data());
                auto const x       = std::stoul(message + 4U, &index);
                auto const y       = std::stoul(message + 5U + index, &index);
                simulation.add(Particle(x, y));
            }
            catch (std::invalid_argument const &)
            {
                std::cerr << "Unable to parse add message\n";
            }
            catch (std::out_of_range const &)
            {
                std::cerr << "Unable to parse add message\n";
            }
        }
        else if (checkForMessage(bytesReceived, "draw", 4))
        {
            simulation.simulate();
            renderer.render();
            socket.sendData(renderer.image());
        }
        else if (checkForMessage(bytesReceived, "close", 4))
        {
            return 0;
        }
    }
    return 0;
}

} // namespace APowers

int main(int argc, char **argv) noexcept { return APowers::realMain(argc, argv); }
