#include "Framework.h"
#include "CommandList.h"
#include "../../Shader/Shader.h"

Command::Command()
{
    constant_buffers.reserve(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
    shader_resources.reserve(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
    samplers.reserve(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
    render_targets.reserve(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);

    Clear();
}

void Command::Clear()
{
    command_type                    = CommandType::Unknown;
    pass_name                       = NOT_ASSIGNED_STR;

    vertex_buffer                   = nullptr;
    vertex_count                    = 0;
    vertex_offset                   = 0;
    vertex_stride                   = 0;

    index_buffer                    = nullptr;
    index_count                     = 0;
    index_offset                    = 0;

    input_layout                    = nullptr;
    primitive_topology              = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    vertex_shader                   = nullptr;
    pixel_shader                    = nullptr;

    constant_buffers.clear();
    constant_buffer_shader_scope    = ShaderStage::Unknown;
    constant_buffer_count           = 0;
    constant_buffer_slot            = 0;

    shader_resources.clear();
    shader_resource_shader_scope    = ShaderStage::Unknown;
    shader_resource_count           = 0;
    shader_resource_slot            = 0;

    samplers.clear();
    sampler_shader_scope            = ShaderStage::Unknown;
    sampler_count                   = 0;
    sampler_slot                    = 0;

    rasterizer_state                = nullptr;
    viewport                        = { 0 };

    blend_state                     = nullptr;
    depth_stencil_state             = nullptr;

    render_targets.clear();
    render_target_count             = 0;

    depth_stencil_target            = nullptr;

    clear_render_target             = nullptr;
    clear_color                     = Color4::Zero;

    clear_depth_stencil_target      = nullptr;
    clear_flags                     = 0;
    clear_stencil                   = 0;
    clear_depth                     = 0.0f;
}

CommandList::CommandList(Context * context)
    : command_capacity(2500)
    , command_count(0)
{
    commands.reserve(command_capacity);
    commands.resize(command_capacity);

    graphics = context->GetSubsystem<Graphics>();
}

CommandList::~CommandList()
{
    Clear();
}

void CommandList::Begin(const std::string & pass_name)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::Begin;
    cmd.pass_name       = pass_name;
}

void CommandList::End()
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::End;
}

void CommandList::Draw(const uint & vertex_count)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::Draw;
    cmd.vertex_count    = vertex_count;
}

void CommandList::DrawIndexed(const uint & index_count, const uint & index_offset, const uint & vertex_offset)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::DrawIndexed;
    cmd.index_count     = index_count;
    cmd.index_offset    = index_offset;
    cmd.vertex_offset   = vertex_offset;
}

void CommandList::SetVertexBuffer(VertexBuffer * buffer)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::SetVertexBuffer;
    cmd.vertex_buffer   = buffer->GetResource();
    cmd.vertex_offset   = buffer->GetOffset();
    cmd.vertex_stride   = buffer->GetStride();
    cmd.vertex_count    = buffer->GetCount();
}

void CommandList::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
{
    SetVertexBuffer(buffer.get());
}

void CommandList::SetIndexBuffer(IndexBuffer * buffer)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::SetIndexBuffer;
    cmd.index_buffer    = buffer->GetResource();
    cmd.index_offset    = buffer->GetOffset();
    cmd.index_count     = buffer->GetCount();
}

void CommandList::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer)
{
    SetIndexBuffer(buffer.get());
}

void CommandList::SetInputLayout(InputLayout * input_layout)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::SetInputLayout;
    cmd.input_layout    = input_layout->GetResource();
}

void CommandList::SetInputLayout(const std::shared_ptr<InputLayout>& input_layout)
{
    SetInputLayout(input_layout.get());
}

void CommandList::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY & primitive_topology)
{
    auto& cmd               = GetCommand();
    cmd.command_type        = CommandType::SetPrimitiveTopology;
    cmd.primitive_topology  = primitive_topology;
}

void CommandList::SetVertexShader(Shader * shader)
{
    SetVertexShader(shader->GetShader<VertexShader>());
}

void CommandList::SetVertexShader(const std::shared_ptr<Shader>& shader)
{
    SetVertexShader(shader.get());
}

void CommandList::SetVertexShader(VertexShader * shader)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::SetVertexShader;
    cmd.vertex_shader   = shader ? shader->GetResource() : nullptr;
}

void CommandList::SetVertexShader(const std::shared_ptr<VertexShader>& shader)
{
    SetVertexShader(shader.get());
}

void CommandList::SetPixelShader(Shader * shader)
{
    SetPixelShader(shader->GetShader<PixelShader>());
}

void CommandList::SetPixelShader(const std::shared_ptr<Shader>& shader)
{
    SetPixelShader(shader.get());
}

void CommandList::SetPixelShader(PixelShader * shader)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::SetPixelShader;
    cmd.pixel_shader    = shader ? shader->GetResource() : nullptr;
}

void CommandList::SetPixelShader(const std::shared_ptr<PixelShader>& shader)
{
    SetPixelShader(shader.get());
}

void CommandList::SetConstantBuffer(const uint & slot, const ShaderStage & stage, ConstantBuffer * buffer)
{
    auto& cmd                           = GetCommand();
    cmd.command_type                    = CommandType::SetConstantBuffer;
    cmd.constant_buffer_slot            = slot;
    cmd.constant_buffer_shader_scope    = stage;
    cmd.constant_buffer_count++;
    cmd.constant_buffers.emplace_back(buffer->GetResource());
}

void CommandList::SetConstantBuffer(const uint & slot, const ShaderStage & stage, const std::shared_ptr<ConstantBuffer>& buffer)
{
    SetConstantBuffer(slot, stage, buffer.get());
}

void CommandList::SetConstantBuffers(const uint & slot, const ShaderStage & stage, const std::vector<ID3D11Buffer*>& buffers)
{
    auto& cmd                           = GetCommand();
    cmd.command_type                    = CommandType::SetConstantBuffer;
    cmd.constant_buffer_slot            = slot;
    cmd.constant_buffer_shader_scope    = stage;
    cmd.constant_buffer_count           = buffers.size();
    cmd.constant_buffers                = buffers;
}

void CommandList::SetShaderResource(const uint & slot, const ShaderStage & stage, Texture * texture)
{
    auto& cmd                           = GetCommand();
    cmd.command_type                    = CommandType::SetShaderResource;
    cmd.shader_resource_slot            = slot;
    cmd.shader_resource_shader_scope    = stage;
    cmd.shader_resource_count++;
    cmd.shader_resources.emplace_back(texture->GetShaderResource());
}

void CommandList::SetShaderResource(const uint & slot, const ShaderStage & stage, const std::shared_ptr<Texture>& texture)
{
    SetShaderResource(slot, stage, texture.get());
}

void CommandList::SetShaderResources(const uint & slot, const ShaderStage & stage, const std::vector<ID3D11ShaderResourceView*>& shader_resources)
{
    auto& cmd                           = GetCommand();
    cmd.command_type                    = CommandType::SetShaderResource;
    cmd.shader_resource_slot            = slot;
    cmd.shader_resource_shader_scope    = stage;
    cmd.shader_resource_count           = shader_resources.size();
    cmd.shader_resources                = shader_resources;
}

void CommandList::SetSamplerState(const uint & slot, const ShaderStage & stage, SamplerState * sampler_state)
{
    auto& cmd                   = GetCommand();
    cmd.command_type            = CommandType::SetSampler;
    cmd.sampler_slot            = slot;
    cmd.sampler_shader_scope    = stage;
    cmd.sampler_count++;
    cmd.samplers.emplace_back(sampler_state ? sampler_state->GetResource() : nullptr);
}

void CommandList::SetSamplerState(const uint & slot, const ShaderStage & stage, const std::shared_ptr<SamplerState>& sampler_state)
{
    SetSamplerState(slot, stage, sampler_state.get());
}

void CommandList::SetSamplerStates(const uint & slot, const ShaderStage & stage, const std::vector<ID3D11SamplerState*>& sampler_states)
{
    auto& cmd                   = GetCommand();
    cmd.command_type            = CommandType::SetSampler;
    cmd.sampler_slot            = slot;
    cmd.sampler_shader_scope    = stage;
    cmd.sampler_count           = sampler_states.size();
    cmd.samplers                = sampler_states;
}

void CommandList::SetViewport(const D3D11_VIEWPORT & viewport)
{
    auto& cmd           = GetCommand();
    cmd.command_type    = CommandType::SetViewport;
    cmd.viewport        = viewport;
}

void CommandList::SetRasterizerState(RasterizerState * rasterizer_state)
{
    auto& cmd               = GetCommand();
    cmd.command_type        = CommandType::SetRasterizerState;
    cmd.rasterizer_state    = rasterizer_state ? rasterizer_state->GetResource() : nullptr;
}

void CommandList::SetRasterizerState(const std::shared_ptr<RasterizerState>& rasterizer_state)
{
    SetRasterizerState(rasterizer_state.get());
}

void CommandList::SetBlendState(BlendState * blend_state)
{
	auto& cmd = GetCommand();
	cmd.command_type = CommandType::SetBlendState;
	cmd.blend_state = blend_state ? blend_state->GetResource() : nullptr;
}

void CommandList::SetBlendState(const std::shared_ptr<BlendState>& blend_state)
{
	SetBlendState(blend_state.get());
}

void CommandList::SetRenderTarget(RenderTexture * render_target, RenderTexture * depth_stencil_target)
{
    auto& cmd                   = GetCommand();
    cmd.command_type            = CommandType::SetRenderTarget;
    cmd.depth_stencil_target    = depth_stencil_target ? depth_stencil_target->GetDepthStencilView() : nullptr;
    cmd.render_targets.emplace_back(render_target ? render_target->GetRenderTargetView() : nullptr);
    cmd.render_target_count++;
}

void CommandList::SetRenderTarget(const std::shared_ptr<RenderTexture>& render_target, const std::shared_ptr<RenderTexture>& depth_stencil_target)
{
    SetRenderTarget(render_target.get(), depth_stencil_target.get());
}

void CommandList::ClearRenderTarget(RenderTexture * render_target, const Color4 & clear_color)
{
    auto& cmd               = GetCommand();
    cmd.command_type        = CommandType::ClearRenderTarget;
    cmd.clear_render_target = render_target ? render_target->GetRenderTargetView() : nullptr;
    cmd.clear_color         = clear_color;
}

void CommandList::ClearRenderTarget(const std::shared_ptr<RenderTexture>& render_target, const Color4 & clear_color)
{
    ClearRenderTarget(render_target.get(), clear_color);
}

auto CommandList::Submit() -> const bool
{
    auto device_context = graphics->GetDeviceContext();

    for (uint i = 0; i < command_count; i++)
    {
        auto& cmd = commands[i];

        switch (cmd.command_type)
        {
        case CommandType::Begin:
            break;
        case CommandType::End:
            break;
        case CommandType::Draw:
            device_context->Draw(cmd.vertex_count, 0);
            break;
        case CommandType::DrawIndexed:
            device_context->DrawIndexed(cmd.index_count, cmd.index_offset, cmd.vertex_offset);
            break;
        case CommandType::SetVertexBuffer:
            device_context->IASetVertexBuffers(0, 1, &cmd.vertex_buffer, &cmd.vertex_stride, &cmd.vertex_offset);
            break;
        case CommandType::SetIndexBuffer:
            device_context->IASetIndexBuffer(cmd.index_buffer, DXGI_FORMAT_R32_UINT, cmd.index_offset);
            break;
        case CommandType::SetInputLayout:
            device_context->IASetInputLayout(cmd.input_layout);
            break;
        case CommandType::SetPrimitiveTopology:
            device_context->IASetPrimitiveTopology(cmd.primitive_topology);
            break;
        case CommandType::SetVertexShader:
            device_context->VSSetShader(cmd.vertex_shader, nullptr, 0);
            break;
        case CommandType::SetPixelShader:
            device_context->PSSetShader(cmd.pixel_shader, nullptr, 0);
            break;
        case CommandType::SetConstantBuffer:
        {
            switch (cmd.constant_buffer_shader_scope)
            {
            case ShaderStage::Global:
            case ShaderStage::VS: device_context->VSSetConstantBuffers(cmd.constant_buffer_slot, cmd.constant_buffer_count, cmd.constant_buffers.data()); if (cmd.constant_buffer_shader_scope != ShaderStage::Global) break;
            case ShaderStage::PS: device_context->PSSetConstantBuffers(cmd.constant_buffer_slot, cmd.constant_buffer_count, cmd.constant_buffers.data()); if (cmd.constant_buffer_shader_scope != ShaderStage::Global) break;
            case ShaderStage::GS: device_context->GSSetConstantBuffers(cmd.constant_buffer_slot, cmd.constant_buffer_count, cmd.constant_buffers.data()); if (cmd.constant_buffer_shader_scope != ShaderStage::Global) break;
            case ShaderStage::CS: device_context->CSSetConstantBuffers(cmd.constant_buffer_slot, cmd.constant_buffer_count, cmd.constant_buffers.data()); if (cmd.constant_buffer_shader_scope != ShaderStage::Global) break;
            }

            break;
        }
        case CommandType::SetShaderResource:
        {
            switch (cmd.shader_resource_shader_scope)
            {
            case ShaderStage::Global:
            case ShaderStage::VS: device_context->VSSetShaderResources(cmd.shader_resource_slot, cmd.shader_resource_count, cmd.shader_resources.data()); if (cmd.shader_resource_shader_scope != ShaderStage::Global) break;
            case ShaderStage::PS: device_context->PSSetShaderResources(cmd.shader_resource_slot, cmd.shader_resource_count, cmd.shader_resources.data()); if (cmd.shader_resource_shader_scope != ShaderStage::Global) break;
            case ShaderStage::GS: device_context->GSSetShaderResources(cmd.shader_resource_slot, cmd.shader_resource_count, cmd.shader_resources.data()); if (cmd.shader_resource_shader_scope != ShaderStage::Global) break;
            case ShaderStage::CS: device_context->CSSetShaderResources(cmd.shader_resource_slot, cmd.shader_resource_count, cmd.shader_resources.data()); if (cmd.shader_resource_shader_scope != ShaderStage::Global) break;
            }

            break;
        }
        case CommandType::SetSampler:
        {
            switch (cmd.sampler_shader_scope)
            {
            case ShaderStage::Global:
            case ShaderStage::VS: device_context->VSSetSamplers(cmd.sampler_slot, cmd.sampler_count, cmd.samplers.data()); if (cmd.sampler_shader_scope != ShaderStage::Global) break;
            case ShaderStage::PS: device_context->PSSetSamplers(cmd.sampler_slot, cmd.sampler_count, cmd.samplers.data()); if (cmd.sampler_shader_scope != ShaderStage::Global) break;
            case ShaderStage::GS: device_context->GSSetSamplers(cmd.sampler_slot, cmd.sampler_count, cmd.samplers.data()); if (cmd.sampler_shader_scope != ShaderStage::Global) break;
            case ShaderStage::CS: device_context->CSSetSamplers(cmd.sampler_slot, cmd.sampler_count, cmd.samplers.data()); if (cmd.sampler_shader_scope != ShaderStage::Global) break;
            }

            break;
        }
        case CommandType::SetRasterizerState:
            break;
        case CommandType::SetViewport:
            device_context->RSSetViewports(1, &cmd.viewport);
            break;
        case CommandType::SetBlendState:
            break;
        case CommandType::SetDepthStencilState:
            break;
        case CommandType::SetRenderTarget:
            device_context->OMSetRenderTargets(cmd.render_target_count, cmd.render_targets.data(), cmd.depth_stencil_target);
            break;
        case CommandType::ClearRenderTarget:
            device_context->ClearRenderTargetView(cmd.clear_render_target, cmd.clear_color);
            break;
        case CommandType::ClearDepthStencilTarget:
            device_context->ClearDepthStencilView(cmd.clear_depth_stencil_target, cmd.clear_flags, cmd.clear_depth, cmd.clear_stencil);
            break;
        }
    }

    Clear();
    return true;
}

auto CommandList::GetCommand() -> Command &
{
    if (command_count >= commands.size())
    {
        uint new_size = command_count + 100;
        commands.reserve(new_size);
        commands.resize(new_size);

        LOG_WARNING_F
        (
            "Command list has grown to fit %d command. Consider making the capacity larger to avoid re-allocations.",
            command_count + 1
        );
    }

    return commands[command_count++];
}

void CommandList::Clear()
{
    for (uint i = 0; i < command_count; i++)
        commands[i].Clear();

    command_count = 0;
}
