#pragma once
#include "framework.h"

namespace Window
{
    static HINSTANCE Instance;
    static HWND Handle;
    static std::function<LRESULT(HWND, uint, WPARAM, LPARAM)> Input_proc;
    static std::function<LRESULT(HWND, uint, WPARAM, LPARAM)> Editor_proc;
    static std::function<void(const uint&, const uint&)> Resize;

    inline LRESULT CALLBACK WndProc
    (
        HWND handle,
        uint message,
        WPARAM wParam,
        LPARAM lParam
    )
    {
        if (Input_proc != nullptr)
            Input_proc(handle, message, wParam, lParam);

        if (Editor_proc != nullptr)
            Editor_proc(handle, message, wParam, lParam);

        switch (message)
        {
        case WM_DISPLAYCHANGE:
        case WM_SIZE:
            if (Resize != nullptr && wParam != SIZE_MINIMIZED)
                Resize(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(handle, message, wParam, lParam);
        }

        return 0;
    }

    inline void Create
    (
        HINSTANCE instance, 
        const uint& width, 
        const uint& height
    )
    {
        Instance = instance;

        WNDCLASSEX wndClass;
        wndClass.cbClsExtra     = 0;
        wndClass.cbWndExtra     = 0;
        wndClass.hbrBackground  = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        wndClass.hCursor        = LoadCursor(nullptr, IDC_ARROW);
        wndClass.hIcon          = LoadIcon(nullptr, IDI_ERROR);
        wndClass.hIconSm        = LoadIcon(nullptr, IDI_ERROR);
        wndClass.hInstance      = instance;
        wndClass.lpfnWndProc    = static_cast<WNDPROC>(WndProc);
        wndClass.lpszClassName  = L"D2D11Game";
        wndClass.lpszMenuName   = nullptr;
        wndClass.style          = CS_HREDRAW | CS_VREDRAW;
        wndClass.cbSize         = sizeof(WNDCLASSEX);

        auto check = RegisterClassEx(&wndClass);
        assert(check != 0);

        Handle = CreateWindowExW
        (
            WS_EX_APPWINDOW,
            L"D2D11Game",
            L"D2D11Game",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            instance,
            nullptr
        );
        assert(Handle != nullptr);

        RECT rect;
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = static_cast<LONG>(width);
        rect.bottom = static_cast<LONG>(height);

        uint centerX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
        uint centerY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        MoveWindow
        (
            Handle,
            centerX,
            centerY,
            rect.right - rect.left,
            rect.bottom - rect.top,
            TRUE
        );
    }

    inline void Show()
    {
        SetForegroundWindow(Handle);
        SetFocus(Handle);
        ShowCursor(TRUE);
        ShowWindow(Handle, SW_NORMAL);
        UpdateWindow(Handle);
    }

    inline const bool Update()
    {
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return msg.message != WM_QUIT;
    }

    inline void Destroy()
    {
        DestroyWindow(Handle);
        UnregisterClass(L"D2D11Game", Instance);
    }

    inline const uint GetWidth()
    {
        RECT rect;
        GetClientRect(Handle, &rect);
        return static_cast<uint>(rect.right - rect.left);
    }

    inline const uint GetHeight()
    {
        RECT rect;
        GetClientRect(Handle, &rect);
        return static_cast<uint>(rect.bottom - rect.top);
    }

    inline void Settings()
    {
        Settings::Get().SetWindowInstance(Instance);
        Settings::Get().SetWindowHandle(Handle);
        Settings::Get().SetWidth(static_cast<float>(GetWidth()));
        Settings::Get().SetHeight(static_cast<float>(GetHeight()));
    }
}
