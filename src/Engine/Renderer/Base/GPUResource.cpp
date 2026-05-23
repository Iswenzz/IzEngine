#include "GPUResource.hpp"

namespace IzEngine
{
	void GPUResource::OnBeforeReset()
	{
		Release();
	}

	void GPUResource::OnAfterReset() { }

	void GPUResource::RegisterResource(GPUResource* resource)
	{
		IZ_ASSERT(resource, "Resource is null.");
		Resources.push_back(resource);
	}

	void GPUResource::UnregisterResource(GPUResource* resource)
	{
		IZ_ASSERT(resource, "Resource is null.");
		Resources.erase(std::remove(Resources.begin(), Resources.end(), resource), Resources.end());
	}

	void GPUResource::NotifyBeforeReset()
	{
		for (auto& resource : Resources)
			resource->OnBeforeReset();
	}

	void GPUResource::NotifyAfterReset()
	{
		for (auto& resource : Resources)
			resource->OnAfterReset();
	}
}
