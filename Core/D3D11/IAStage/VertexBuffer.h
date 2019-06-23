#pragma once
#include "../IPipeline.h"

class VertexBuffer final : public IPipeline
{
public:
    VertexBuffer(class Context* context);
    ~VertexBuffer();

    auto GetResource() const -> ID3D11Buffer* { return buffer; }
    auto GetOffset() const -> const uint& { return offset; }
    auto GetStride() const -> const uint& { return stride; }
    auto GetCount() const -> const uint& { return vertexCount; }

    template <typename T>
    void Create
    (
        const std::vector<T>& vertices,
        const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT
    );

    void Clear();
    void BindPipeline();

private:
    class Graphics* graphics;

    ID3D11Buffer* buffer;
    uint stride;
    uint offset;
    uint vertexCount;
};

template<typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertices, const D3D11_USAGE & usage)
{
    stride = sizeof(T);
    vertexCount = vertices.size();

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

    switch (usage)
    {
    case D3D11_USAGE_IMMUTABLE:
    case D3D11_USAGE_DEFAULT:
        desc.CPUAccessFlags = 0;
        break;
    case D3D11_USAGE_DYNAMIC:
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    case D3D11_USAGE_STAGING:
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
        break;
    }

    desc.Usage = usage;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.ByteWidth = stride * vertices.size();

    D3D11_SUBRESOURCE_DATA subData;
    ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
    subData.pSysMem = vertices.data();

    auto hr = graphics->GetDevice()->CreateBuffer
    (
        &desc,
        &subData,
        &buffer
    );
    assert(SUCCEEDED(hr));
}
