#pragma once
#include "../IPipeline.h"

class SamplerState final : public IPipeline
{
public:
    SamplerState(class Context* context);
    ~SamplerState();

    auto GetResource() const -> ID3D11SamplerState* { return state; }

    void Create
    (
        const D3D11_FILTER& filter,
        const D3D11_TEXTURE_ADDRESS_MODE& address_mode,
        const D3D11_COMPARISON_FUNC& comparison_func = D3D11_COMPARISON_ALWAYS
    );
    void Clear();
    void BindPipeline(const ShaderStage& stage, const uint& slot);

private:
    class Graphics* graphics;
    ID3D11SamplerState* state;
};