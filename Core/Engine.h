#pragma once

enum EngineFlags : uint
{
    EngineFlags_Update  = 1U << 0,
    EngineFlags_Render  = 1U << 1,
    EngineFlags_Game    = 1U << 2,
};

class Engine final
{
public:
    static auto GetEngineFlags() -> const uint& { return engine_flags; }
    static void SetEngineFlags(const uint& flags) { engine_flags = flags; }

    static void EngineFlagsEnable(const EngineFlags& flag) { engine_flags |= flag; }
    static void EngineFlagsDisable(const EngineFlags& flag) { engine_flags &= ~flag; }
    static void EngineFlagsToggle(const EngineFlags& flag)
    {
        IsEngineFlagsEnable(flag) ? EngineFlagsDisable(flag) : EngineFlagsEnable(flag);
    }
    static auto IsEngineFlagsEnable(const EngineFlags& flag) -> const bool
    {
        return (engine_flags & flag) > 0U;
    }

public:
    Engine();
    ~Engine();

    auto GetContext() const -> class Context* { return context; }

    void Update();

private:
    static uint engine_flags;

    class Context* context;
};