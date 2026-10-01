#pragma once
#include "XRCapture.hpp"

#include "Engine/Backend/OpenXR/D3D11.hpp"

namespace IzEngine
{
	// One frame on its way back from the GPU: the system memory copies of what was captured, and the
	// layers it was drawn for, so it is submitted with the poses that match its pixels.
	struct DX9XRSlot
	{
		IDirect3DSurface9* Eyes[2] = {};
		IDirect3DSurface9* Panel = nullptr;
		IDirect3DQuery9* Done = nullptr;
		bool Pending = false;
		XRLayers Layers;
	};

	// Carries what a D3D9 renderer draws to OpenXR's Direct3D 11 swapchains. A legacy D3D9 device cannot
	// share a texture with D3D11, so the pixels go through system memory, read back a frame late so
	// neither side waits on the other.
	class API DX9XRBridge : public DX9XRCapture
	{
	public:
		DX9XRBridge(XRD3D11& graphics);
		~DX9XRBridge() override;

		void Queue(const XRLayers& layers);
		bool Submit(XRLayers& layers);
		bool Deliver(const XRLayers& layers, XRLayers& ready) override;

	private:
		XRD3D11& Graphics;
		DX9XRSlot Slots[2];
		int Current = 0;
		std::vector<uint8_t> Swizzled;

		void ReleaseCaptures() override;
		void Resized() override;
		void CreateSlot(DX9XRSlot& slot);
		void ReleaseSlot(DX9XRSlot& slot);
		bool Upload(XRSwapchain& swapchain, IDirect3DSurface9* surface, int height);
	};
}
