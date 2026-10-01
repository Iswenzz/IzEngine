#include "D3D12.hpp"
#include "OpenXR.hpp"

namespace IzEngine
{
	namespace
	{
		// BGRA only: frames reach these swapchains by copies on the GPU, which cannot swap channels. sRGB
		// first, since renderers write gamma encoded bytes, which an sRGB format takes as they are.
		constexpr XRFormat Preferred[] = {
			{ DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, false },
			{ DXGI_FORMAT_B8G8R8A8_UNORM, false },
		};

		template <typename T>
		void SafeRelease(T*& object)
		{
			if (object)
				object->Release();
			object = nullptr;
		}
	}

	const char* XRD3D12::Extension() const
	{
		return XR_KHR_D3D12_ENABLE_EXTENSION_NAME;
	}

	bool XRD3D12::Initialize(XrInstance instance, XrSystemId system, std::string& error)
	{
		const auto requirementsFor = reinterpret_cast<PFN_xrGetD3D12GraphicsRequirementsKHR>(
			OpenXR::Function("xrGetD3D12GraphicsRequirementsKHR"));

		// Required before a session can be made, and it names the adapter the device has to live on.
		XrGraphicsRequirementsD3D12KHR requirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_D3D12_KHR };
		if (!requirementsFor || XR_FAILED(requirementsFor(instance, system, &requirements)))
		{
			error = "the runtime gave no Direct3D 12 adapter";
			return false;
		}
		if (!CreateDevice(requirements.adapterLuid, requirements.minFeatureLevel, error))
			return false;

		SessionBinding.device = Device;
		SessionBinding.queue = Queue;
		return true;
	}

	void XRD3D12::Shutdown()
	{
		SafeRelease(Queue);
		SafeRelease(Device);
		SessionBinding.device = nullptr;
		SessionBinding.queue = nullptr;
	}

	// Direct3D 12 is loaded when asked for, so a system without it still runs everything else.
	bool XRD3D12::CreateDevice(LUID adapter, D3D_FEATURE_LEVEL level, std::string& error)
	{
		if (Device)
			return true;

		const HMODULE module = LoadLibraryA("d3d12.dll");
		const auto create =
			module ? reinterpret_cast<PFN_D3D12_CREATE_DEVICE>(GetProcAddress(module, "D3D12CreateDevice")) : nullptr;
		if (!create)
		{
			error = "Direct3D 12 is unavailable";
			return false;
		}

		IDXGIAdapter1* match = XRAdapter(adapter);
		if (!match)
		{
			error = "the headset's GPU was not found";
			return false;
		}
		const HRESULT hr = create(match, level, IID_PPV_ARGS(&Device));
		match->Release();
		if (FAILED(hr))
		{
			error = std::format("D3D12CreateDevice failed ({:#x})", static_cast<uint32_t>(hr));
			Device = nullptr;
			return false;
		}

		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		if (FAILED(Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&Queue))))
		{
			error = "the Direct3D 12 queue could not be made";
			Queue = nullptr;
			SafeRelease(Device);
			return false;
		}
		return true;
	}

	const void* XRD3D12::Binding() const
	{
		return &SessionBinding;
	}

	std::span<const XRFormat> XRD3D12::Formats() const
	{
		return Preferred;
	}

	bool XRD3D12::Images(XrSwapchain swapchain, std::vector<uint64_t>& images)
	{
		uint32_t count = 0;
		xrEnumerateSwapchainImages(swapchain, 0, &count, nullptr);
		std::vector<XrSwapchainImageD3D12KHR> list(count, { XR_TYPE_SWAPCHAIN_IMAGE_D3D12_KHR });
		if (XR_FAILED(xrEnumerateSwapchainImages(swapchain, count, &count, reinterpret_cast<XrSwapchainImageBaseHeader*>(list.data()))))
			return false;

		images.clear();
		for (const auto& image : list)
			images.push_back(reinterpret_cast<uint64_t>(image.texture));
		return !images.empty();
	}
}
