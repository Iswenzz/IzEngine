#include "Device.hpp"

namespace IW3SR::Engine
{
	void Device::Assign(IDirect3DDevice9* device)
	{
		D3Device = device;
	}
}
