#pragma once
#include "Engine/Backends/DX9.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Graphics device.
	/// </summary>
	class Device : public IObject
	{
		CLASS_SINGLETON(Device)
	public:
		IDirect3DDevice9* D3Device = nullptr;

		/// <summary>
		/// Assign a device.
		/// </summary>
		/// <param name="device">The device.</param>
		void Assign(IDirect3DDevice9* device);

	private:
		/// <summary>
		/// Initialize the device.
		/// </summary>
		Device() = default;
		virtual ~Device() = default;
	};
}
