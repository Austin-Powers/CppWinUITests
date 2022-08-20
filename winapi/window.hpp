#ifndef CPPWINUITEST_WINAPI_WINDOW_H
#define CPPWINUITEST_WINAPI_WINDOW_H

#include <Windows.h>

namespace APowers {

/// @brief The base class for all windows.
///
/// @tparam TDerived The type derived from this base.
template <typename TDerived>
class Window
{
public:
    /// @brief The window procedure.
    ///
    /// @param windowHandle The handle of the window the procedure was called for.
    /// @param message The message code.
    /// @param wParam The additional parameter for the message.
    /// @param lParam The additional parameter for the message.
    /// @return The response to the message.
    static LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept
    {
        TDerived *instance = nullptr;

        if (message == WM_NCCREATE)
        {
            auto create = reinterpret_cast<CREATESTRUCT *>(lParam);
            instance    = reinterpret_cast<TDerived *>(create->lpCreateParams);
            SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));

            instance->_hwnd = windowHandle;
        }
        else
        {
            instance = reinterpret_cast<TDerived *>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
        }

        if (instance)
        {
            return instance->handleMessage(message, wParam, lParam);
        }
        else
        {
            return DefWindowProc(windowHandle, message, wParam, lParam);
        }
    }

    /// @brief Default initializes the Window.
    Window() noexcept : _hwnd{nullptr} {}

    /// @brief Creates the window.
    ///
    /// @param windowName The name of the window, displayed in the frame.
    /// @param style The style of the window.
    /// @param exStyle The additional window style.
    /// @param x The x coordinate of the starting location of the window on the screen.
    /// @param y The y coordinate of the starting location of the window on the screen.
    /// @param width The starting width of the window on the screen.
    /// @param height the starting height of the window in the screen.
    /// @param parentHandle The handle of the parent window, if there is one.
    /// @param menuHandle The handle of the menu, if there is one.
    /// @return True if the window was created, false otherwise.
    bool create(PCWSTR windowName,
                DWORD  style,
                DWORD  exStyle      = 0,
                int    x            = CW_USEDEFAULT,
                int    y            = CW_USEDEFAULT,
                int    width        = CW_USEDEFAULT,
                int    height       = CW_USEDEFAULT,
                HWND   parentHandle = 0,
                HMENU  hMenu        = 0)
    {
        WNDCLASS wc = {0};

        wc.lpfnWndProc   = TDerived::WindowProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.lpszClassName = className();

        RegisterClass(&wc);

        _hwnd = CreateWindowEx(exStyle,
                               className(),
                               windowName,
                               style,
                               x,
                               y,
                               width,
                               height,
                               parentHandle,
                               hMenu,
                               GetModuleHandle(NULL),
                               this);

        return _hwnd != 0;
    }

    /// @brief Returns the handle of this window.
    ///
    /// @return The handle of this window.
    HWND window() const noexcept { return _hwnd; }

protected:
    /// @brief Returns the name of the window class.
    ///
    /// @return The name of the window class.
    virtual PCWSTR className() const noexcept = 0;

    /// @brief Method called for handling message to the window.
    ///
    /// @param message The message to handle.
    /// @param wParam The additional parameter of the message.
    /// @param lParam The additional parameter of the message.
    /// @return The response for the system.
    virtual LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam) noexcept = 0;

    /// @brief The handle of this window.
    HWND _hwnd;
};

} // namespace APowers

#endif // !CPPWINUITEST_WINAPI_WINDOW_H
