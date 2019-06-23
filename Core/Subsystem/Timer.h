#pragma once
#include "ISubsystem.h"

class Timer final : public ISubsystem
{
public:
    Timer(class Context* context);
    ~Timer() = default;

    const bool Initialize() override;

    auto GetDeltaTimeMs() const -> const float& { return deltaTimeMs; }
    auto GetDeltaTimeSec() const -> const float& { return deltaTimeSec; }

    void Update();

private:
    float deltaTimeMs;
    float deltaTimeSec;

    std::chrono::high_resolution_clock::time_point prevTime;
};