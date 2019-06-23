#include "Framework.h"
#include "Audio.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include "../../Scene/Component/Transform.h"

Audio::Audio(Context * context)
    : ISubsystem(context)
    , system(nullptr)
    , listener(nullptr)
    , max_channels(32)
    , distance_factor(1.0f)
    , bInitialized(false)
    , result(0)
{
    EventSystem::Get().Subscibe(EventType::Update, EVENT_HANDLER(Update));
}

Audio::~Audio()
{
    if (!system)
        return;

    result = system->close();
    if (result != FMOD_OK)
    {
        LogErrorFMOD(result);
        return;
    }

    result = system->release();
    if (result != FMOD_OK)
    {
        LogErrorFMOD(result);
        return;
    }
}

const bool Audio::Initialize()
{
    //Create FMOD System
    result = System_Create(&system);
    if (result != FMOD_OK)
    {
        LogErrorFMOD(result);
        return false;
    }

    //Check Version
    uint version = 0;
    result = system->getVersion(&version);
    if (result != FMOD_OK)
    {
        LogErrorFMOD(result);
        return false;
    }

    if (version < FMOD_VERSION)
    {
        LogErrorFMOD(result);
        return false;
    }

    //Check sound card devices
    auto driver_count = 0;
    result = system->getNumDrivers(&driver_count);
    if (result != FMOD_OK)
    {
        LogErrorFMOD(result);
        return false;
    }

    //Initialize FMOD
    result = system->init(max_channels, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK)
    {
        LogErrorFMOD(result);
        return false;
    }

    //3D Settings
    result = system->set3DSettings(1.0f, distance_factor, 0.0f);
    if (result != FMOD_OK)
    {
        LogErrorFMOD(result);
        return false;
    }

    bInitialized = true;

    return true;
}

void Audio::SetListenerTransform(Transform * transform)
{
    listener = transform;
}

void Audio::Update()
{
    if (!Engine::IsEngineFlagsEnable(EngineFlags_Game))
        return;

    if (!bInitialized)
        return;

    result = system->update();
    if (result != FMOD_OK)
    {
        LogErrorFMOD(result);
        return;
    }

    if (listener)
    {
        auto position   = listener->GetTransform();
        auto velocity   = Vector3(0.0f, 0.0f, 0.0f);
        auto forward    = Vector3::Forward;
        auto up         = Vector3::Up;

        result = system->set3DListenerAttributes
        (
            0,
            reinterpret_cast<FMOD_VECTOR*>(&position),
            reinterpret_cast<FMOD_VECTOR*>(&velocity),
            reinterpret_cast<FMOD_VECTOR*>(&forward),
            reinterpret_cast<FMOD_VECTOR*>(&up)
        );
        if (result != FMOD_OK)
        {
            LogErrorFMOD(result);
            return;
        }

    }
}

void Audio::LogErrorFMOD(const int & error) const
{
    LOG_ERROR(std::string(FMOD_ErrorString(static_cast<FMOD_RESULT>(error))));
}
