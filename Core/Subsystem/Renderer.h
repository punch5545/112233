#pragma once
#include "ISubsystem.h"

enum class RenderableType : uint
{
    Opaque,
    Transparent,
    Camera,
};

class Renderer final : public ISubsystem
{
public:
    Renderer(class Context* context);
    ~Renderer() = default;

    const bool Initialize() override;

    auto GetFrameResourceView() const -> ID3D11ShaderResourceView*;
    auto GetCamera() const -> class Camera*;

    auto GetResolution() const -> const Vector2& { return resolution; }
    void SetResolution(const uint& width, const uint& height);

    void AcquireRenderables(class Scene* scene);
    void ClearRenderables();

	void SetRasterizerState(bool bShowWireframe) { if(this->bShowWireframe != bShowWireframe) this->bShowWireframe = bShowWireframe; }
	void SetBlendState(bool bShowBackground) { if(this->bShowBackground != bShowBackground) this->bShowBackground = bShowBackground; }

	void Render();

private:
    void PassMain();

    void PassOpaque();

private:
    void CreateRenderTextures();
    void CreateShaders();
    void CreateConstantBuffer();
    void CreateSamplerStates();
	void CreateRasterizerStates();
	void CreateBlendStates();

    void UpdateGlobalBuffer(const uint& width, const uint& height, const Matrix& wvp = Matrix::Identity);


private:
    float camera_near;
    float camera_far;
    Matrix camera_view;
    Matrix camera_proj;
    Matrix camera_view_proj;
    Matrix camera_view_proj_inverse;
    Vector3 camera_position;
    Vector2 resolution;

	bool bShowWireframe;
	bool bShowBackground;

    class Camera* scene_camera;
    std::shared_ptr<class Camera> editor_camera;

    std::shared_ptr<class CommandList> command_list;
    std::unordered_map<RenderableType, std::vector<class Actor*>> renderables;

private:
#pragma region RenderTexture
    //Full
    std::shared_ptr<class RenderTexture> full_hdr_target1;
    std::shared_ptr<class RenderTexture> full_hdr_target2;
#pragma endregion
#pragma region Shader
    std::shared_ptr<class Shader> vs_standard;
    std::shared_ptr<class Shader> vs_animation;
#pragma endregion
#pragma region ConstantBuffer
    std::shared_ptr<class ConstantBuffer> global_buffer;  
#pragma endregion
#pragma region SamplerState
    std::shared_ptr<class SamplerState> point_clamp;
    std::shared_ptr<class SamplerState> bilinear_clamp;
    std::shared_ptr<class SamplerState> bilinear_wrap;
    std::shared_ptr<class SamplerState> trilinear_clamp;
#pragma endregion
#pragma region RasterizerState
    std::shared_ptr<class RasterizerState> cull_none_solid;
    std::shared_ptr<class RasterizerState> cull_back_solid;
    std::shared_ptr<class RasterizerState> cull_front_solid;
    std::shared_ptr<class RasterizerState> cull_none_wireframe;
    std::shared_ptr<class RasterizerState> cull_back_wireframe;
    std::shared_ptr<class RasterizerState> cull_front_wireframe;
#pragma endregion

#pragma region BlendState
	std::shared_ptr<class BlendState> blend_enable;
	std::shared_ptr<class BlendState> blend_disable;
#pragma endregion
};