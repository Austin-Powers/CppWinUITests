#ifndef CPPWINUITEST_CSHARPCONNECTOR_SIMPLESOCKET_H
#define CPPWINUITEST_CSHARPCONNECTOR_SIMPLESOCKET_H

#include <chrono>
#include <gsl/gsl>

namespace APowers {

/// @brief A simple server socket maintaining a single connection.
class SimpleSocket final
{
public:
    /// @brief Initializes a new SimpleSocket waiting for a connection on the given port.
    ///
    /// @param port The port to wait for a connection.
    SimpleSocket(std::uint16_t const port) noexcept;

    // Prevent copying and moving by deleting constructor and assignment operator
    SimpleSocket(SimpleSocket const &) noexcept            = delete;
    SimpleSocket(SimpleSocket &&) noexcept                 = delete;
    SimpleSocket &operator=(SimpleSocket const &) noexcept = delete;
    SimpleSocket &operator=(SimpleSocket &&) noexcept      = delete;

    /// @brief Finalizes the SimpleSocket closing the connection.
    ~SimpleSocket() noexcept;

    /// @brief Blocks the caller while waiting for a connection on the given port.
    void connect() noexcept;

    /// @brief Checks if the socket is connected to something.
    ///
    /// @return True if the socket is connected, false otherwise.
    bool isConnected() const noexcept;

    /// @brief Closes the socket.
    void close() noexcept;

    /// @brief Receives data via the connection.
    ///
    /// @param data The buffer to store the data in.
    /// @param timeout The duration after which the receiving shall timeout.
    /// @return A subspan of the given span containing the received data.
    gsl::span<std::uint8_t> receiveData(gsl::span<std::uint8_t> data, std::chrono::milliseconds const &timeout) noexcept;

    /// @brief Sends the given data.
    ///
    /// @param data The buffer containing to send.
    void sendData(gsl::span<std::uint8_t const> data) noexcept;

private:
    /// @brief The socket type.
    using socket_t = unsigned long long;

    /// @brief Flag signalling if WSA is running.
    bool _wsaRunning{};

    /// @brief The port to listen for a connection.
    std::uint16_t _port{};

    /// @brief The socket of the connection.
    socket_t _socket{};

    /// @brief Timeout value of the last receive call.
    std::chrono::milliseconds _lastTimoutValue{};
};

} // namespace APowers

#endif // !CPPWINUITEST_CSHARPCONNECTOR_SIMPLESOCKET_H
