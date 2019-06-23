#pragma once

class Settings final
{
public:
    static Settings& Get()
    {
        static Settings instance;
        return instance;
    }

    auto GetWindowInstance() const -> HINSTANCE { return hInstance; }
    void SetWindowInstance(HINSTANCE hInstance) { this->hInstance = hInstance; }

    auto GetWindowHandle() const -> HWND { return handle; }
    void SetWindowHandle(HWND handle) { this->handle = handle; }

    auto GetWidth() const -> const float { return windowSize.x; }
    void SetWidth(const float& width) { this->windowSize.x = width; }

    auto GetHeight() const -> const float { return windowSize.y; }
    void SetHeight(const float& height) { this->windowSize.y = height; }

    auto IsVsync() const -> const bool& { return bVsync; }
    void SetIsVsync(const bool& bVsync) { this->bVsync = bVsync; }

private:
    Settings();
    ~Settings() = default;

private:
    HINSTANCE hInstance;
    HWND handle;
    Vector2 windowSize;
    bool bVsync; 
};
// Vertical Synchronization - 수직 동기화
// - Tearing 방지
// - 과도한 CPU, GPU 연산 중지
// - 컴퓨터 성능이 안좋으면 화면 안정

// - 60프레임에서 조금만 프레임이 떨여져도 성능이 반토막남
// - Input Lag