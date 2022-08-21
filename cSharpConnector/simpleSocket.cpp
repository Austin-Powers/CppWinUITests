#include "simpleSocket.h"

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>

namespace APowers {

SimpleSocket::SimpleSocket(std::uint16_t const port) noexcept : _port{port}, _socket{INVALID_SOCKET}
{
    WSADATA wsaData;
    auto    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    _wsaRunning    = (result == 0);
}

SimpleSocket::~SimpleSocket() noexcept
{
    close();
    WSACleanup();
}

void SimpleSocket::connect() noexcept
{
    close();
    if (_wsaRunning)
    {
        _socket = [this]() noexcept -> socket_t {
            auto serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (serverSocket == INVALID_SOCKET)
            {
                return INVALID_SOCKET;
            }
            sockaddr_in service{};
            service.sin_family = AF_INET;
            if (InetPton(AF_INET, "127.0.0.1", &service.sin_addr.s_addr) != 1)
            {
                closesocket(serverSocket);
                return INVALID_SOCKET;
            }
            service.sin_port = htons(_port);
            if (bind(_socket, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR)
            {
                closesocket(serverSocket);
                return INVALID_SOCKET;
            }

            if (listen(_socket, SOMAXCONN) == SOCKET_ERROR)
            {
                closesocket(serverSocket);
                return INVALID_SOCKET;
            }
            auto const result = accept(serverSocket, nullptr, nullptr);
            closesocket(serverSocket);
            return result;
        }();
    }
}

bool SimpleSocket::isConnected() const noexcept { return _socket != INVALID_SOCKET; }

void SimpleSocket::close() noexcept
{
    if (_socket != INVALID_SOCKET)
    {
        closesocket(_socket);
        _socket = INVALID_SOCKET;
    }
}

gsl::span<std::uint8_t> SimpleSocket::receiveData(gsl::span<std::uint8_t>          data,
                                                  std::chrono::milliseconds const &timeout) noexcept
{
    if (!isConnected())
    {
        return {};
    }

    if (_lastTimoutValue != timeout)
    {
        _lastTimoutValue  = timeout;
        auto timeoutValue = static_cast<DWORD>(timeout.count());
        auto result =
            setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&timeoutValue), sizeof DWORD);
        if (result != 0)
        {
            return {};
        }
    }

    auto result = recv(_socket, reinterpret_cast<char *>(data.data()), static_cast<int>(data.size()), 0);
    if (result > 0)
    {
        // success
        return data.subspan(0, result);
    }
    if (result == 0)
    {
        // connection closed
        close();
        return {};
    }
    auto error = WSAGetLastError();
    if (error == WSAETIMEDOUT)
    {
        // timeout
        return {};
    }
    // connection error
    return {};
}

void SimpleSocket::sendData(gsl::span<std::uint8_t const> data) noexcept
{
    if (isConnected())
    {
        send(_socket, reinterpret_cast<const char *>(data.data()), static_cast<int>(data.size()), 0);
    }
}

} // namespace APowers
