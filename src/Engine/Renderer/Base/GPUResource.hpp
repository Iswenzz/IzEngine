#pragma once
#include "Engine/Core/Common.hpp"

namespace IzEngine
{
	class API GPUResource : public IResource
	{
	public:
		virtual ~GPUResource() = default;

		virtual void OnBeforeReset();
		virtual void OnAfterReset();

		static void RegisterResource(GPUResource* resource);
		static void UnregisterResource(GPUResource* resource);

		static void NotifyBeforeReset();
		static void NotifyAfterReset();

	private:
		static inline std::vector<GPUResource*> Resources;
	};
}
