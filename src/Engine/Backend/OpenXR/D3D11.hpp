#pragma once
#include "Graphics.hpp"

#include <d3d11.h>
#include <dxgi.h>

#define XR_USE_GRAPHICS_API_D3D11
#include <openxr/openxr_platform.h>

namespace IzEngine
{
	// OpenXR through Direct3D 11: a device made on the adapter the runtime composites on.
	class API XRD3D11 : public XRGraphics
	{
	public:
		ID3D11Device* Device = nullptr;
		ID3D11DeviceContext* Context = nullptr;

		const char* Extension() const override;
		bool Initialize(XrInstance instance, XrSystemId system, std::string& error) override;
		void Shutdown() override;
		const void* Binding() const override;
		std::span<const XRFormat> Formats() const override;
		bool Images(XrSwapchain swapchain, std::vector<void*>& images) override;

	private:
		XrGraphicsBindingD3D11KHR SessionBinding{ XR_TYPE_GRAPHICS_BINDING_D3D11_KHR };

		bool CreateDevice(LUID adapter, D3D_FEATURE_LEVEL level, std::string& error);
	};
}
