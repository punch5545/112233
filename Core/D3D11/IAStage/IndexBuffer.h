#pragma once
#include "../IPipeline.h"

class IndexBuffer final : public IPipeline
{
public:
    IndexBuffer(class Context* context);
    ~IndexBuffer();

    auto GetResource() const -> ID3D11Buffer* { return buffer; }
    auto GetOffset() const -> const uint& { return offset; }
    auto GetCount() const -> const uint& { return indexCount; }

    void Create
    (
        const std::vector<uint>& indices,
        const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT
    );
    void Clear();
    void BindPipeline();

private:
    class Graphics* graphics;

    ID3D11Buffer* buffer;
    uint offset;
    uint indexCount;
};