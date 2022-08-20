#ifndef UNICODE
#define UNICODE
#endif

#include <d2d1_3.h>
#include <windows.h>
#pragma comment(lib, "d2d1")

#include "helpers.h"
#include "renderer/renderer.h"
#include "simulation/simulation.h"
#include "window.hpp"

#include <atomic>
#include <memory>
#include <thread>

/// @brief The class of the simulation window.
///
/// @remarks
/// https://docs.microsoft.com/en-us/windows/win32/Direct2D/how-to-draw-a-bitmap
/// https://docs.microsoft.com/en-us/windows/win32/api/_direct2d/
class SimulationWindow : public APowers::Window<SimulationWindow>
{
public:
    /// @brief Initializes a new SimulationWindow.
    SimulationWindow() : factory(nullptr), renderTarget(nullptr), bitmap{nullptr}
    {
        _thread = std::thread([this]() { threadMethod(); });
    }

    /// @brief Finalizes the class, shuting down the thread.
    ~SimulationWindow() noexcept
    {
        _terminate = true;
        if (_thread.joinable())
        {
            _thread.join();
        }
    }

    /// @brief Returns the name of the window class.
    ///
    /// @return The name of the window class.
    PCWSTR className() const noexcept override { return L"Circle Window Class"; }

    /// @brief Method called for handling message to the window.
    ///
    /// @param message The message to handle.
    /// @param wParam The additional parameter of the message.
    /// @param lParam The additional parameter of the message.
    /// @return The response for the system.
    LRESULT SimulationWindow::handleMessage(UINT message, WPARAM wParam, LPARAM lParam) noexcept override
    {
        switch (message)
        {
        case WM_CREATE: {
            if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory)))
            {
                return -1; // Fail CreateWindowEx.
            }
            return 0;
        }
        case WM_DESTROY: {
            releaseGraphicsResources();
            APowers::safeRelease(&factory);
            PostQuitMessage(0);
            return 0;
        }
        case WM_PAINT: {
            onPaint();
            return 0;
        }
        case WM_SIZE: {
            Resize();
            return 0;
        }
        }
        return DefWindowProc(_hwnd, message, wParam, lParam);
    }

private:
    /// @brief Creates the graphics resources for rendering, if they do not already exist.
    ///
    /// @return The result of the operation.
    HRESULT SimulationWindow::createGraphicsResources() noexcept
    {
        HRESULT hr = S_OK;
        if (renderTarget == nullptr)
        {
            RECT rc;
            GetClientRect(_hwnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            hr = factory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(_hwnd, size), &renderTarget);

            if (SUCCEEDED(hr))
            {
                renderer.render();
                D2D1_SIZE_U bitmapSize{};
                bitmapSize.width  = simulation.cellSize() * simulation.columns();
                bitmapSize.height = simulation.cellSize() * simulation.rows();

                D2D1_BITMAP_PROPERTIES properties{};
                // https://docs.microsoft.com/en-us/windows/win32/direct2d/supported-pixel-formats-and-alpha-modes#supported-formats-for-id2d1hwndrendertarget
                properties.pixelFormat.format    = DXGI_FORMAT_B8G8R8A8_UNORM;
                properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

                hr = renderTarget->CreateBitmap(bitmapSize,
                                                reinterpret_cast<void const *>(renderer.image().data() + 54U),
                                                (bitmapSize.width * 4U),
                                                &properties,
                                                &bitmap);
            }
        }
        return hr;
    }

    /// @brief Releases the graphics resources.
    void SimulationWindow::releaseGraphicsResources() noexcept
    {
        APowers::safeRelease(&bitmap);
        APowers::safeRelease(&renderTarget);
    }

    /// @brief Handler method for WM_PAINT messages.
    void SimulationWindow::onPaint() noexcept
    {
        HRESULT hr = createGraphicsResources();
        if (SUCCEEDED(hr))
        {
            PAINTSTRUCT ps;
            BeginPaint(_hwnd, &ps);

            renderTarget->BeginDraw();

            renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
            if (bitmap)
            {
                D2D1_RECT_F rect{};
                auto const  size = renderTarget->GetSize();

                rect.right  = size.width;
                rect.bottom = size.height;
                renderTarget->DrawBitmap(bitmap, &rect);
            }
            hr = renderTarget->EndDraw();
            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            {
                releaseGraphicsResources();
            }
            EndPaint(_hwnd, &ps);
        }
    }

    /// @brief Handler method for WM_RESIZE messages.
    void SimulationWindow::Resize() noexcept
    {
        if (renderTarget != nullptr)
        {
            RECT rc;
            GetClientRect(_hwnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            renderTarget->Resize(size);
            InvalidateRect(_hwnd, nullptr, FALSE);
        }
    }

    /// @brief The method executet by the thread.
    void threadMethod() noexcept
    {
        while (!_terminate)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{100U});
            if (renderTarget != nullptr)
            {
                RECT rc;
                GetClientRect(_hwnd, &rc);
                RedrawWindow(_hwnd, &rc, nullptr, RDW_INTERNALPAINT);
            }
        }
    }

    /// @brief The simulation.
    APowers::Simulation simulation{20U, 16U, 9U};

    /// @brief The renderer for generating images from the simulation.
    APowers::Renderer renderer{simulation};

    /// @brief The factory for Direct2D components.
    ID2D1Factory *factory;

    /// @brief The window render target.
    ID2D1HwndRenderTarget *renderTarget;

    /// @brief The bitmap to render.
    ID2D1Bitmap *bitmap;

    /// @brief The flag signaling the shutdown of the class.
    std::atomic_bool _terminate{};

    /// @brief The thread calling sending WM_PAINT messages.
    std::thread _thread{};
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    auto const win = std::make_unique<SimulationWindow>();

    if (!win->create(L"Simulation", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win->window(), nCmdShow);

    // Run the message loop.

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
