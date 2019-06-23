#pragma once
#include "ISubsystem.h"

namespace FMOD
{
    class System;
}

class Audio final : public ISubsystem
{
public:
    Audio(class Context* context);
    ~Audio();

    const bool Initialize() override;

    auto GetSystemFMOD() const ->FMOD::System* { return system; }

    void SetListenerTransform(class Transform* transform);

    void Update();

private:
    void LogErrorFMOD(const int& error) const;

private:
    FMOD::System* system;
    class Transform* listener;

    uint max_channels;
    float distance_factor;
    bool bInitialized;
    int result;
};