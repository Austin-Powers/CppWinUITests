#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC         hdc = BeginPaint(windowHandle, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(windowHandle, &ps);
    }
        return 0;
    }
    return DefWindowProc(windowHandle, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // setup and register window class needed for call to CreateWindowEx
    wchar_t const className[] = L"Simulation Window Class";

    WNDCLASS windowClass      = {};
    windowClass.lpfnWndProc   = WindowProc;
    windowClass.hInstance     = instanceHandle;
    windowClass.lpszClassName = className;
    RegisterClass(&windowClass);

    // create the window
    auto windowHandle = CreateWindowEx(0,
                                       className,
                                       L"Particle Simulation",
                                       WS_OVERLAPPEDWINDOW,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       nullptr,
                                       nullptr,
                                       instanceHandle,
                                       nullptr);

    if (windowHandle == nullptr)
    {
        return 0;
    }

    ShowWindow(windowHandle, nCmdShow);

    MSG message = {};
    while (GetMessage(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}
