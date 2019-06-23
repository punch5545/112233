#pragma once
#include "../IPipeline.h"

class RasterizerState final : public IPipeline
{
public:
    RasterizerState(class Context* context);
    ~RasterizerState();

    auto GetResource() const -> ID3D11RasterizerState* { return state; }

    void Create
    (
        const D3D11_CULL_MODE& cull_mode,
        const D3D11_FILL_MODE& fill_mode
    );
    void Clear();
    void BindPipeline();

private:
    class Graphics* graphics;
    ID3D11RasterizerState* state;
};