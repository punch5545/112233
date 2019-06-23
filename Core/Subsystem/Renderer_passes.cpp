#include "Framework.h"
#include "Renderer.h"
#include "../D3D11/CommandList.h"
#include "../../Shader/Shader.h"
#include "../../Scene/Scene.h"
#include "../../Scene/Actor.h"
#include "../../Scene/Component/Camera.h"
#include "../../Scene/Component/Transform.h"
#include "../../Scene/Component/Renderable.h"
#include "../../Scene/Component/Animator.h"

void Renderer::PassMain()
{
    command_list->Begin("PassMain");

    PassOpaque();

    command_list->End();
    command_list->Submit();
}

void Renderer::PassOpaque()
{
    command_list->Begin("PassOpaque");

    if (renderables[RenderableType::Opaque].empty())
    {
        command_list->ClearRenderTarget(full_hdr_target2, 1.0f);
        command_list->End();
        command_list->Submit();
        return;
    }

    UpdateGlobalBuffer(static_cast<uint>(resolution.x), static_cast<uint>(resolution.y));

    command_list->SetRenderTarget(full_hdr_target2);
    command_list->SetViewport(full_hdr_target2->GetViewport());
    command_list->ClearRenderTarget(full_hdr_target2, 1.0f); 
    command_list->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    command_list->SetInputLayout(vs_standard->GetInputLayout());
    command_list->SetConstantBuffer(0, ShaderStage::Global, global_buffer);
	command_list->SetSamplerState(0, ShaderStage::PS, trilinear_clamp);
	command_list->SetRasterizerState(bShowWireframe ? cull_none_wireframe : cull_none_solid);
	command_list->SetBlendState(bShowBackground ? blend_disable : blend_enable);

    uint current_material   = 0;
    uint current_mesh       = 0;
    uint current_shader     = 0;

    for (const auto& actor : renderables[RenderableType::Opaque])
    {
        auto renderable     = actor->GetComponent<Renderable>();
        auto material       = renderable    ? renderable->GetMaterial() : nullptr;
        auto mesh           = renderable    ? renderable->GetMesh()     : nullptr;
        auto shader         = material      ? material->GetShader()     : nullptr;
        auto vertex_buffer  = mesh          ? mesh->GetVertexBuffer()   : nullptr;
        auto index_buffer   = mesh          ? mesh->GetIndexBuffer()    : nullptr;

        if (!renderable || !material || !mesh || !shader || !vertex_buffer || !index_buffer)
            continue;

        //Vertex Shader
        if (auto animator = actor->GetComponent<Animator>())
        {
            animator->UpdateConstantBuffer();
            command_list->SetVertexShader(vs_animation);
            command_list->SetConstantBuffer(3, ShaderStage::VS, animator->GetConstnatBuffer());
        }
        else
            command_list->SetVertexShader(vs_standard);

        //Mesh
        if (current_mesh != mesh->GetResourceID())
        {
            command_list->SetVertexBuffer(vertex_buffer);
            command_list->SetIndexBuffer(index_buffer);
            current_mesh = mesh->GetResourceID();
        }

        //Pixel Shader
        if (current_shader != shader->GetShaderID())
        {
            command_list->SetPixelShader(shader);
            current_shader = shader->GetShaderID();
        }

        //Material
        if (current_material!= material->GetResourceID())
        {
            std::vector<ID3D11ShaderResourceView*> textures
            {
                material->GetTextureShaderResource(TextureType::Albedo),
                material->GetTextureShaderResource(TextureType::Normal),
                material->GetTextureShaderResource(TextureType::Emissive),
            };

            material->UpdateConstantBuffer();

            command_list->SetConstantBuffer(1, ShaderStage::PS, material->GetConstantBuffer());
            command_list->SetShaderResources(0, ShaderStage::PS, textures);

            current_material = material->GetResourceID();
        }

        auto transform = actor->GetTransform();
        transform->UpdateConstantBuffer(camera_view_proj);

        command_list->SetConstantBuffer(2, ShaderStage::VS, transform->GetConstantBuffer());

        command_list->DrawIndexed
        (
            index_buffer->GetCount(),
            index_buffer->GetOffset(),
            vertex_buffer->GetOffset()
        );
    }

    command_list->End();
    command_list->Submit();
}
