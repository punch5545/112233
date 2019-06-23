#pragma once
#include "../IPipeline.h"

class InputLayout final : public IPipeline
{
public:
    InputLayout(class Context* context);
    ~InputLayout();

    auto GetResource() const -> ID3D11InputLayout* { return inputLayout; }

    void Create
    (
        D3D11_INPUT_ELEMENT_DESC* descs,
        const uint& count,
        ID3DBlob* blob
    );

    void Create(ID3DBlob* blob);

    void Clear();
    void BindPipeline();

private:
    class Graphics* graphics;
    ID3D11InputLayout* inputLayout;
};