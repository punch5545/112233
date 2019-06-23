#include "Framework.h"
#include "Renderer.h"
#include "../D3D11/CommandList.h"
#include "../../Shader/Shader.h"
#include "../../Scene/Scene.h"
#include "../../Scene/Actor.h"
#include "../../Scene/Component/Camera.h"
#include "../../Scene/Component/Transform.h"
#include "../../Scene/Component/Renderable.h"

Renderer::Renderer(Context * context)
    : ISubsystem(context)  
    , scene_camera(nullptr)
	, bShowBackground(false)
	, bShowWireframe(true)
{
    EventSystem::Get().Subscibe(EventType::Render, EVENT_HANDLER(Render));

    resolution.x = Settings::Get().GetWidth();
    resolution.y = Settings::Get().GetHeight();

}

const bool Renderer::Initialize()
{
    editor_camera = std::make_shared<Camera>(context);

    command_list = std::make_shared<CommandList>(context);

    CreateRenderTextures();
    CreateShaders();
    CreateConstantBuffer();
    CreateSamplerStates();
	CreateRasterizerStates();
	CreateBlendStates();

    return true;
}

auto Renderer::GetFrameResourceView() const -> ID3D11ShaderResourceView *
{
    return full_hdr_target2 ? full_hdr_target2->GetShaderResourceView() : nullptr;
}

auto Renderer::GetCamera() const -> class Camera *
{
    return Engine::IsEngineFlagsEnable(EngineFlags_Game) ? scene_camera : editor_camera.get();
}

void Renderer::SetResolution(const uint & width, const uint & height)
{
    if (width == 0 || height == 0)
    {
        LOG_ERROR_F("%dx%d is an invalid resolution.", width, height);
        return;
    }

    if (resolution.x == width && resolution.y == height)
        return;

    resolution.x = static_cast<float>((width  % 2 != 0) ? width  - 1 : width);
    resolution.y = static_cast<float>((height % 2 != 0) ? height - 1 : height);

    CreateRenderTextures();

    LOG_INFO_F
    (
        "Resolution set to %dx%d", 
        static_cast<uint>(resolution.x), 
        static_cast<uint>(resolution.x)
    );
}

void Renderer::AcquireRenderables(Scene * scene)
{
    ClearRenderables();

    auto actors = scene->GetActors();
    for (auto& actor : actors)
    {
        auto renderable = actor->GetComponent<Renderable>();
        auto camera     = actor->GetComponent<Camera>();

        if (renderable)
            renderables[RenderableType::Opaque].emplace_back(actor);

        if (camera)
        {
            renderables[RenderableType::Camera].emplace_back(actor);
            scene_camera = camera;
        }
    }
}

void Renderer::ClearRenderables()
{
    scene_camera = nullptr;
    renderables.clear();
}

void Renderer::Render()
{
    auto camera = GetCamera();
    if (!camera)
        return;

    if (renderables.empty())
        return;

    camera->UpdateEditorCamera();

    camera_view                 = camera->GetViewMatrix();
    camera_proj                 = camera->GetProjMatrix();
    camera_view_proj            = camera_view * camera_proj;
    camera_view_proj_inverse    = camera_view_proj.Inverse();
    camera_position             = camera->GetTransform()->GetTranslation();
    camera_near                 = camera->GetNearPlane();
    camera_far                  = camera->GetFarPlane();

    PassMain();
}

void Renderer::CreateRenderTextures()
{
    auto width  = static_cast<uint>(resolution.x);
    auto height = static_cast<uint>(resolution.y);

    //Full
    full_hdr_target1 = std::make_shared<RenderTexture>(context);
    full_hdr_target1->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

    full_hdr_target2 = std::make_shared<RenderTexture>(context);
    full_hdr_target2->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void Renderer::CreateShaders()
{
    vs_standard = std::make_shared<Shader>(context);
    vs_standard->AddShader<VertexShader>("../../_Assets/Shader/Standard.hlsl");

    vs_animation = std::make_shared<Shader>(context);
    vs_animation->AddDefine("ANIMATION");
    vs_animation->AddShader<VertexShader>("../../_Assets/Shader/Standard.hlsl");
}

void Renderer::CreateConstantBuffer()
{
    global_buffer = std::make_shared<ConstantBuffer>(context);
    global_buffer->Create<GlobalData>();
}

void Renderer::CreateSamplerStates()
{
    point_clamp = std::make_shared<SamplerState>(context);
    point_clamp->Create(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_BORDER);

    bilinear_clamp = std::make_shared<SamplerState>(context);
    bilinear_clamp->Create(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);

    bilinear_wrap = std::make_shared<SamplerState>(context);
    bilinear_wrap->Create(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);

    trilinear_clamp = std::make_shared<SamplerState>(context);
    trilinear_clamp->Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
}

void Renderer::CreateRasterizerStates()
{
    cull_none_solid = std::make_shared<RasterizerState>(context);
    cull_none_solid->Create(D3D11_CULL_NONE, D3D11_FILL_SOLID);

    cull_back_solid = std::make_shared<RasterizerState>(context);
    cull_back_solid->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);

    cull_front_solid = std::make_shared<RasterizerState>(context);
    cull_front_solid->Create(D3D11_CULL_FRONT, D3D11_FILL_SOLID);

    cull_none_wireframe = std::make_shared<RasterizerState>(context);
    cull_none_wireframe->Create(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);

    cull_back_wireframe = std::make_shared<RasterizerState>(context);
    cull_back_wireframe->Create(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);

    cull_front_wireframe = std::make_shared<RasterizerState>(context);
    cull_front_wireframe->Create(D3D11_CULL_FRONT, D3D11_FILL_WIREFRAME);
}

void Renderer::CreateBlendStates()
{
	blend_enable = std::make_shared<BlendState>(context);
	blend_enable->Create(true);

	blend_disable = std::make_shared<BlendState>(context);
	blend_enable->Create(false);
}
void Renderer::UpdateGlobalBuffer(const uint & width, const uint & height, const Matrix & wvp)
{    
    auto data = global_buffer->Map<GlobalData>();
    if (!data)
    {
        LOG_ERROR("Failed to map buffer");
        return;
    }

    data->wvp                       = wvp;
    data->camera_view               = camera_view;
    data->camera_proj               = camera_proj;
    data->camera_view_proj          = camera_view_proj;
    data->camera_view_proj_inverse  = camera_view_proj_inverse;
    data->camera_position           = camera_position;
    data->camera_far                = camera_far;
    data->camera_near               = camera_near;
    data->resolution                = Vector2(static_cast<float>(width), static_cast<float>(height));

    global_buffer->Unmap();
}