#include "DLL.hpp"
#include "Sys/Log.hpp"

#include <stdexcept>

namespace IW3SR
{
	DLL::DLL(std::string filePath)
	{
		FilePath = filePath;
		Instance = LoadLibrary(filePath.c_str());
		Mode = DLLMode::DEFAULT;

		if (!Instance)
			throw std::runtime_error("Couldn't load DLL.");

		Initialize = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "Initialize"));
		GUI = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "GUI"));
		Shutdown = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "Shutdown"));

		if (!Initialize)
			throw std::runtime_error("DLL is missing Initialize function.");

		Initialize(this);
	}

	DLL::~DLL()
	{
		if (Mode == DLLMode::PERSISTED)
			return;

		if (Shutdown)
			Shutdown();

		FreeLibrary(Instance);
	}
}
