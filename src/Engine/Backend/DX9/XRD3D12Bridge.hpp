#pragma once
#include "XRCapture.hpp"

#include "Engine/Backend/OpenXR/D3D12.hpp"

#include <d3d9on12.h>

namespace IzEngine
{
	// The commands of one frame's copies, and the fence value that says when they may be recorded again.
	struct DX9XRCopy
	{
		ID3D12CommandAllocator* Allocator = nullptr;
		ID3D12GraphicsCommandList* List = nullptr;
		UINT64 Done = 0;
	};

	// Carries what a D3D9 renderer draws to OpenXR's Direct3D 12 swapchains. The renderer's device is made by
	// D3D9On12 on the session's own D3D12 device, so the captures are copied on the GPU, in the frame they
	// were drawn, and never leave it.
	class API DX9XRD3D12Bridge : public DX9XRCapture
	{
	public:
		DX9XRD3D12Bridge(XRD3D12& graphics);
		~DX9XRD3D12Bridge() override;

		static IDirect3D9* CreateDirect3D(XRD3D12& graphics, UINT sdkVersion);

		bool Attach(IDirect3DDevice9* device) override;
		bool Deliver(const XRLayers& layers, XRLayers& ready) override;
		bool Present(HWND window) override;

	private:
		XRD3D12& Graphics;
		IDirect3DDevice9On12* On12 = nullptr;
		IDirect3DQuery9* Flush = nullptr;
		ID3D12Fence* Fence = nullptr;
		HANDLE FenceEvent = nullptr;
		UINT64 FenceValue = 0;
		std::array<DX9XRCopy, 3> Copies;
		size_t Next = 0;
		HWND Child = nullptr;
		IDXGISwapChain3* Swapchain = nullptr;
		glm::ivec2 ChildSize{};

		DX9XRCopy* NextCopy();
		void Wait();
		void ReleaseCaptures() override;
	};
}
