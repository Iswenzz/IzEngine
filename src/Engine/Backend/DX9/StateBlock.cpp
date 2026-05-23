#include "StateBlock.hpp"
#include "GraphicsContext.hpp"

namespace IzEngine
{
	DX9StateBlock::DX9StateBlock()
	{
		GPUResource::RegisterResource(this);
	}

	DX9StateBlock::~DX9StateBlock()
	{
		GPUResource::UnregisterResource(this);
		Release();
	}

	void DX9StateBlock::Capture()
	{
		if (!StateBlock)
			DX9GraphicsContext::Device->CreateStateBlock(D3DSBT_ALL, &StateBlock);

		StateBlock->Capture();
	}

	void DX9StateBlock::Apply()
	{
		if (StateBlock)
			StateBlock->Apply();
	}

	void DX9StateBlock::Release()
	{
		if (StateBlock)
		{
			StateBlock->Release();
			StateBlock = nullptr;
		}
	}

	void DX9StateBlock::OnBeforeReset()
	{
		Release();
	}

	void DX9StateBlock::OnAfterReset() { }
}
