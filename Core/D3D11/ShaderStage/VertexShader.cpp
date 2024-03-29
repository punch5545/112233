#include "framework.h"
#include "VertexShader.h"
#include "../D3D11_Helper.h"

VertexShader::VertexShader(Context * context)
    : shader(nullptr)
    , blob(nullptr)
    , path("")
    , entryPoint("")
    , shaderModel("")
{
    graphics = context->GetSubsystem<Graphics>();
}

VertexShader::~VertexShader()
{
    Clear();
}

void VertexShader::Create(const std::string & path, const std::string & entryPoint, const std::string & shaderModel, D3D_SHADER_MACRO * defines)
{    
    this->path          = path;
    this->entryPoint    = entryPoint    != NOT_ASSIGNED_STR ? entryPoint    : "VS";
    this->shaderModel   = shaderModel   != NOT_ASSIGNED_STR ? shaderModel   : "vs_5_0";

    auto result = D3D11_Helper::CompileShader
    (
        this->path,
        this->entryPoint,
        this->shaderModel,
        defines,
        &blob
    );

    auto hr = graphics->GetDevice()->CreateVertexShader
    (
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        nullptr,
        &shader
    );
    assert(SUCCEEDED(hr));
}

void VertexShader::Clear()
{
    SAFE_RELEASE(shader);
    SAFE_RELEASE(blob);
}

void VertexShader::BindPipeline()
{
    graphics->GetDeviceContext()->VSSetShader(shader, nullptr, 0);
}
