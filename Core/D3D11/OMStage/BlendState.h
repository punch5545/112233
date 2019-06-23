#pragma once
#include "../IPipeline.h"

class BlendState final : public IPipeline
{
public:
	BlendState(class Context* context);
	~BlendState();

	auto GetResource() const -> ID3D11BlendState* { return state; }

	void Create(const bool& bEnable);
	void Clear();
	void BindPipeline();

private:
	class Graphics* graphics;
	ID3D11BlendState* state;
	uint sample_mask;
};
