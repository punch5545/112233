#pragma once
#include "../IPipeline.h"

class VertexShader final : public IPipeline
{
public:
    VertexShader(class Context* context);
    ~VertexShader();

    auto GetResource() const -> ID3D11VertexShader* { return shader; }
    auto GetBlob() const -> ID3DBlob* { return blob; }

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
    ID3D11VertexShader* shader;
    ID3DBlob* blob;
    std::string path;
    std::string entryPoint;
    std::string shaderModel;
};