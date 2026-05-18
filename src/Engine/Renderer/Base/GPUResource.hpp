#pragma once
#include "Engine/Core/Common.hpp"

namespace IzEngine
{
	class API GPUResource : public IResource
	{
	public:
		virtual ~GPUResource() = default;

		virtual void OnDeviceLost();
		virtual void OnDeviceReset();

		static void RegisterResource(GPUResource* resource);
		static void UnregisterResource(GPUResource* resource);

		static void NotifyDeviceLost();
		static void NotifyDeviceReset();

	private:
		static inline std::vector<GPUResource*> Resources;
	};
}
