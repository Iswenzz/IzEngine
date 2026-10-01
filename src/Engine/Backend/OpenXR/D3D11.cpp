#include "D3D11.hpp"
#include "OpenXR.hpp"

namespace IzEngine
{
	namespace
	{
		// sRGB first: renderers write gamma encoded bytes, which an sRGB format takes as they are.
		constexpr XRFormat Preferred[] = {
			{ DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, false },
			{ DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, true },
			{ DXGI_FORMAT_B8G8R8A8_UNORM, false },
			{ DXGI_FORMAT_R8G8B8A8_UNORM, true },
		};

		template <typename T>
		void SafeRelease(T*& object)
		{
			if (object)
				object->Release();
			object = nullptr;
		}
	}

	const char* XRD3D11::Extension() const
	{
		return XR_KHR_D3D11_ENABLE_EXTENSION_NAME;
	}

	bool XRD3D11::Initialize(XrInstance instance, XrSystemId system, std::string& error)
	{
		const auto requirementsFor = reinterpret_cast<PFN_xrGetD3D11GraphicsRequirementsKHR>(
			OpenXR::Function("xrGetD3D11GraphicsRequirementsKHR"));

		// Required before a session can be made, and it names the adapter the device has to live on.
		XrGraphicsRequirementsD3D11KHR requirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR };
		if (!requirementsFor || XR_FAILED(requirementsFor(instance, system, &requirements)))
		{
			error = "the runtime gave no Direct3D 11 adapter";
			return false;
		}
		if (!CreateDevice(requirements.adapterLuid, requirements.minFeatureLevel, error))
			return false;

		SessionBinding.device = Device;
		return true;
	}

	void XRD3D11::Shutdown()
	{
		SafeRelease(Context);
		SafeRelease(Device);
		SessionBinding.device = nullptr;
	}

	// The runtime composites on one GPU and only takes swapchain images made on that same one.
	bool XRD3D11::CreateDevice(LUID adapter, D3D_FEATURE_LEVEL level, std::string& error)
	{
		if (Device)
			return true;

		IDXGIFactory1* factory = nullptr;
		if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory))))
		{
			error = "DXGI is unavailable";
			return false;
		}

		IDXGIAdapter1* match = nullptr;
		IDXGIAdapter1* candidate = nullptr;
		for (UINT i = 0; factory->EnumAdapters1(i, &candidate) != DXGI_ERROR_NOT_FOUND; i++)
		{
			DXGI_ADAPTER_DESC1 desc = {};
			candidate->GetDesc1(&desc);
			if (desc.AdapterLuid.LowPart == adapter.LowPart && desc.AdapterLuid.HighPart == adapter.HighPart)
			{
				match = candidate;
				break;
			}
			candidate->Release();
		}
		factory->Release();

		if (!match)
		{
			error = "the headset's GPU was not found";
			return false;
		}

		const D3D_FEATURE_LEVEL all[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0 };
		std::vector<D3D_FEATURE_LEVEL> levels;
		for (const D3D_FEATURE_LEVEL candidateLevel : all)
		{
			if (candidateLevel >= level)
				levels.push_back(candidateLevel);
		}

		const auto create = [&](const D3D_FEATURE_LEVEL* list, UINT count) {
			return D3D11CreateDevice(match, D3D_DRIVER_TYPE_UNKNOWN, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, list,
				count, D3D11_SDK_VERSION, &Device, nullptr, &Context);
		};
		HRESULT hr = create(levels.data(), static_cast<UINT>(levels.size()));

		// A Direct3D 11.0 runtime refuses the whole list when it names 11.1.
		if (hr == E_INVALIDARG && levels.size() > 1 && levels.front() == D3D_FEATURE_LEVEL_11_1)
			hr = create(levels.data() + 1, static_cast<UINT>(levels.size() - 1));
		match->Release();

		if (FAILED(hr))
		{
			error = std::format("D3D11CreateDevice failed ({:#x})", static_cast<uint32_t>(hr));
			Device = nullptr;
			Context = nullptr;
			return false;
		}
		return true;
	}

	const void* XRD3D11::Binding() const
	{
		return &SessionBinding;
	}

	std::span<const XRFormat> XRD3D11::Formats() const
	{
		return Preferred;
	}

	bool XRD3D11::Images(XrSwapchain swapchain, std::vector<void*>& images)
	{
		uint32_t count = 0;
		xrEnumerateSwapchainImages(swapchain, 0, &count, nullptr);
		std::vector<XrSwapchainImageD3D11KHR> list(count, { XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR });
		if (XR_FAILED(xrEnumerateSwapchainImages(swapchain, count, &count, reinterpret_cast<XrSwapchainImageBaseHeader*>(list.data()))))
			return false;

		images.clear();
		for (const auto& image : list)
			images.push_back(image.texture);
		return !images.empty();
	}
}
