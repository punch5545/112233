#pragma once
#include "framework.h"
#include "./Subsystem/ISubsystem.h"

class Context final
{
public:
    Context() = default;
    ~Context()
    {
        for (int i = static_cast<int>(subsystems.size() - 1); i >= 0; i--)
            SAFE_DELETE(subsystems[i]);
    }

    auto Intialize() -> const bool
    {
        auto result = true;
        for (auto& subsystem : subsystems)
        {
            if (!subsystem->Initialize())
            {
                result = false;
            }
        }
        return result;
    }

    template <typename T>
    T* RegisterSubsystem();

    template <typename T>
    T* GetSubsystem();

private:
    std::vector<ISubsystem*> subsystems;
};

template<typename T>
inline T * Context::RegisterSubsystem()
{
    static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type dose not implement Isubsystem");
    return static_cast<T*>(subsystems.emplace_back(new T(this)));
}

template<typename T>
inline T * Context::GetSubsystem()
{
    static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type dose not implement Isubsystem");
    for (auto& subsystem : subsystems)
    {
        if (typeid(T) == typeid(*subsystem))
            return static_cast<T*>(subsystem);
    }
    return nullptr;
}
