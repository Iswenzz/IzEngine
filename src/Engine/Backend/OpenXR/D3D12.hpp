#pragma once
#include "Direct3D.hpp"

namespace IzEngine
{
	// OpenXR through Direct3D 12: a device and its queue made on the adapter the runtime composites on.
	class API XRD3D12 : public XRGraphics
	{
	public:
		ID3D12Device* Device = nullptr;
		ID3D12CommandQueue* Queue = nullptr;

		const char* Extension() const override;
		bool Initialize(XrInstance instance, XrSystemId system, std::string& error) override;
		void Shutdown() override;
		const void* Binding() const override;
		std::span<const XRFormat> Formats() const override;
		bool Images(XrSwapchain swapchain, std::vector<uint64_t>& images) override;

	private:
		XrGraphicsBindingD3D12KHR SessionBinding{ XR_TYPE_GRAPHICS_BINDING_D3D12_KHR };

		bool CreateDevice(LUID adapter, D3D_FEATURE_LEVEL level, std::string& error);
	};
}
