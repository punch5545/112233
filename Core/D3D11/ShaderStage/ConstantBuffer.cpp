#include "framework.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(Context * context)
    : buffer(nullptr)
{
    graphics = context->GetSubsystem<Graphics>();
}

ConstantBuffer::~ConstantBuffer()
{
    Clear();
}

void ConstantBuffer::Clear()
{
    SAFE_RELEASE(buffer);
}

void ConstantBuffer::BindPipeline(const ShaderStage & stage, const uint & slot)
{
    switch (stage)
    {
    case ShaderStage::VS: graphics->GetDeviceContext()->VSSetConstantBuffers(slot, 1, &buffer); break;
    case ShaderStage::PS: graphics->GetDeviceContext()->PSSetConstantBuffers(slot, 1, &buffer); break;
    case ShaderStage::GS: graphics->GetDeviceContext()->GSSetConstantBuffers(slot, 1, &buffer); break;
    case ShaderStage::CS: graphics->GetDeviceContext()->CSSetConstantBuffers(slot, 1, &buffer); break;
    }
}

void ConstantBuffer::Unmap()
{
    graphics->GetDeviceContext()->Unmap(buffer, 0);
}
