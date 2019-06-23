#include "Framework.h"
#include "SamplerState.h"

SamplerState::SamplerState(Context * context)
    : state(nullptr)
{
    graphics = context->GetSubsystem<Graphics>();
}

SamplerState::~SamplerState()
{
    Clear();
}

void SamplerState::Create(const D3D11_FILTER & filter, const D3D11_TEXTURE_ADDRESS_MODE & address_mode, const D3D11_COMPARISON_FUNC & comparison_func)
{
    D3D11_SAMPLER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
    desc.Filter         = filter;
    desc.AddressU       = address_mode;
    desc.AddressV       = address_mode;
    desc.AddressW       = address_mode;
    desc.MipLODBias     = 0.0f;
    desc.MaxAnisotropy  = 16;
    desc.ComparisonFunc = comparison_func;
    desc.BorderColor[0] = 1.0f;
    desc.BorderColor[1] = 1.0f;
    desc.BorderColor[2] = 0.0f;
    desc.BorderColor[3] = 1.0f;
    desc.MinLOD         = std::numeric_limits<float>::min();
    desc.MaxLOD         = std::numeric_limits<float>::max();

    auto result = SUCCEEDED(graphics->GetDevice()->CreateSamplerState(&desc, &state));
    if (!result)
        LOG_ERROR("Failed to create sampler state");
}

void SamplerState::Clear()
{
    SAFE_RELEASE(state);
}

void SamplerState::BindPipeline(const ShaderStage & stage, const uint & slot)
{
    switch (stage)
    {
    case ShaderStage::VS: graphics->GetDeviceContext()->VSSetSamplers(slot, 1, &state); break;
    case ShaderStage::PS: graphics->GetDeviceContext()->PSSetSamplers(slot, 1, &state); break;
    case ShaderStage::GS: graphics->GetDeviceContext()->GSSetSamplers(slot, 1, &state); break;
    case ShaderStage::CS: graphics->GetDeviceContext()->CSSetSamplers(slot, 1, &state); break;
    }
}
