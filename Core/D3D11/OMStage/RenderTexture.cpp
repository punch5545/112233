#include "Framework.h"
#include "RenderTexture.h"

RenderTexture::RenderTexture(Context * context)
    : rtv(nullptr)
    , dsv(nullptr)
    , srv(nullptr)
    , format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
    graphics = context->GetSubsystem<Graphics>();

    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
}

RenderTexture::~RenderTexture()
{
    Clear();
}

void RenderTexture::Create(const uint & width, const uint & height, const DXGI_FORMAT & format, const bool & bDepth_enable)
{
    if (width == 0 || height == 0)
    {
        LOG_ERROR("Invalid parameter");
        return;
    }

    this->format            = format;
    this->bDepth_enable     = bDepth_enable;
    this->viewport.TopLeftY = 0.0f;
    this->viewport.TopLeftY = 0.0f;
    this->viewport.Width    = static_cast<float>(width);
    this->viewport.Height   = static_cast<float>(height);
    this->viewport.MinDepth = 0.0f;
    this->viewport.MaxDepth = 1.0f;

    ID3D11Texture2D* texture = nullptr;

    if (bDepth_enable)
    {

    }
    else
    {
        //Create RenderTarget Texture
        {
            D3D11_TEXTURE2D_DESC desc;
            ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
            desc.Width              = width;
            desc.Height             = height;
            desc.MipLevels          = 1;
            desc.ArraySize          = 1;
            desc.Format             = format;
            desc.SampleDesc.Count   = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage              = D3D11_USAGE_DEFAULT;
            desc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags     = 0;
            desc.MiscFlags          = 0;

            auto result = SUCCEEDED(graphics->GetDevice()->CreateTexture2D(&desc, nullptr, &texture));
            if (!result)
            {
                LOG_ERROR("Failed to creat RenderTarget texture");
                return;
            }
        }

        //Create RenderTargetView
        {
            D3D11_RENDER_TARGET_VIEW_DESC desc;
            ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
            desc.Format             = format;
            desc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipSlice = 0;

            auto result = SUCCEEDED(graphics->GetDevice()->CreateRenderTargetView(texture, &desc, &rtv));
            if (!result)
            {
                LOG_ERROR("Failed to creat RenderTargetView");
                SAFE_RELEASE(texture);
                return;
            }
        }

        //Create ShaderResourceView
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC desc;
            ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
            desc.Format                     = format;
            desc.ViewDimension              = D3D11_SRV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipLevels        = 1;
            desc.Texture2D.MostDetailedMip  = 0;

            auto result = SUCCEEDED(graphics->GetDevice()->CreateShaderResourceView(texture, &desc, &srv));
            if (!result)
            {
                LOG_ERROR("Failed to creat ShaderResourceView");
                SAFE_RELEASE(texture);
                return;
            }
        }
    }

    SAFE_RELEASE(texture);
}

void RenderTexture::Clear()
{
    SAFE_RELEASE(srv);
    SAFE_RELEASE(dsv);
    SAFE_RELEASE(rtv);

    format = DXGI_FORMAT_R8G8B8A8_UNORM;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
}
