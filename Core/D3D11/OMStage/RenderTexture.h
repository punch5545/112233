#pragma once
#include "../IPipeline.h"

class RenderTexture final : public IPipeline
{
public:
    RenderTexture(class Context* context);
    ~RenderTexture();

    auto GetShaderResourceView() const -> ID3D11ShaderResourceView* { return srv; }
    auto GetRenderTargetView() const -> ID3D11RenderTargetView* { return rtv; }
    auto GetDepthStencilView() const -> ID3D11DepthStencilView* { return dsv; }

    auto GetFormat() const -> const DXGI_FORMAT& { return format; }
    auto GetViewport() const -> const D3D11_VIEWPORT& { return viewport; }
    auto GetWidth() const -> const uint { return static_cast<uint>(viewport.Width); }
    auto GetHeight() const -> const uint { return static_cast<uint>(viewport.Height); }

    void Create
    (
        const uint& width,
        const uint& height,
        const DXGI_FORMAT& format,
        const bool& bDepth_enable = false
    );
    void Clear();

private:
    class Graphics* graphics;
    ID3D11RenderTargetView* rtv;
    ID3D11DepthStencilView* dsv;
    ID3D11ShaderResourceView* srv;

    D3D11_VIEWPORT viewport;
    DXGI_FORMAT format;

    bool bDepth_enable;
};