#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Base/GPUResource.hpp"

namespace IzEngine
{
	class API DX9StateBlock : public GPUResource
	{
	public:
		DX9StateBlock();
		~DX9StateBlock() override;

		void Capture();
		void Apply();

		void Release() override;
		void OnBeforeReset() override;
		void OnAfterReset() override;

	private:
		IDirect3DStateBlock9* StateBlock = nullptr;
	};
}
