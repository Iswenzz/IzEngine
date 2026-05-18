#include "GPUResource.hpp"

namespace IzEngine
{
	void GPUResource::OnDeviceLost() { }

	void GPUResource::OnDeviceReset() { }

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

	void GPUResource::NotifyDeviceLost()
	{
		for (auto& resource : Resources)
			resource->OnDeviceLost();
	}

	void GPUResource::NotifyDeviceReset()
	{
		for (auto& resource : Resources)
			resource->OnDeviceReset();
	}
}
