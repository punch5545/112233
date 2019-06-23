#pragma once
#include "../IPipeline.h"

class PixelShader final : public IPipeline
{
public:
    PixelShader(class Context* context);
    ~PixelShader();

    auto GetResource() const -> ID3D11PixelShader* { return shader; }
    auto GetBlob() const -> ID3DBlob* { return nullptr; }

    void Create
    (
        const std::string& path,
        const std::string& entryPoint,
        const std::string& shaderModel,
        D3D_SHADER_MACRO* defines = nullptr
    );
    void Clear();
    void BindPipeline();

private:
    class Graphics* graphics;
    ID3D11PixelShader* shader;
    std::string path;
    std::string entryPoint;
    std::string shaderModel;
};