#include "framework.h"
#include "Engine.h"

uint Engine::engine_flags = 0U;

Engine::Engine()
{
    engine_flags = EngineFlags_Update | EngineFlags_Render;

    context = new Context();
    context->RegisterSubsystem<Timer>();
    context->RegisterSubsystem<Audio>();
    context->RegisterSubsystem<Input>();
    context->RegisterSubsystem<Thread>();
    context->RegisterSubsystem<Graphics>();
    context->RegisterSubsystem<Scripting>();
    context->RegisterSubsystem<ResourceManager>();
    context->RegisterSubsystem<Renderer>();
    context->RegisterSubsystem<SceneManager>();

    context->Intialize(); 
}

Engine::~Engine()
{
    SAFE_DELETE(context);
}

void Engine::Update()
{
    if (IsEngineFlagsEnable(EngineFlags_Update))
        EventSystem::Get().Fire(EventType::Update);

    if (IsEngineFlagsEnable(EngineFlags_Render))
        EventSystem::Get().Fire(EventType::Render);
}