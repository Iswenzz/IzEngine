#include "Direct3D.hpp"

namespace IzEngine
{
	// The runtime composites on one GPU and only takes swapchain images made on that same one.
	IDXGIAdapter1* XRAdapter(LUID adapter)
	{
		IDXGIFactory1* factory = nullptr;
		if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory))))
			return nullptr;

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
		return match;
	}
}
