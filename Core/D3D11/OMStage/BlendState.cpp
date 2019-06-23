#include "Framework.h"
#include "BlendState.h"

BlendState::BlendState(Context * context)
	: state(nullptr)
	, sample_mask(0xffffffff)
{
	graphics = context->GetSubsystem<Graphics>();
}

BlendState::~BlendState()
{
	Clear();
}

void BlendState::Create(const bool& bEnable)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;


	desc.RenderTarget[0].BlendEnable = bEnable;

	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	auto result = SUCCEEDED(graphics->GetDevice()->CreateBlendState(&desc, &state));
	if (!result)
		LOG_ERROR("Failed to create blend state");
}

void BlendState::Clear()
{
	SAFE_RELEASE(state);
}

void BlendState::BindPipeline()
{
	graphics->GetDeviceContext()->OMSetBlendState(state,nullptr,sample_mask);
}
