#include "framework.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Context * context)
    : buffer(nullptr)
    , stride(0)
    , offset(0)
    , vertexCount(0)
{
    graphics = context->GetSubsystem<Graphics>();
}

VertexBuffer::~VertexBuffer()
{
    Clear();
}

void VertexBuffer::Clear()
{
    SAFE_RELEASE(buffer);
    stride      = 0;
    offset      = 0;
    vertexCount = 0;
}

void VertexBuffer::BindPipeline()
{
    graphics->GetDeviceContext()->IASetVertexBuffers
    (
        0,
        1,
        &buffer,
        &stride,
        &offset
    );
}
