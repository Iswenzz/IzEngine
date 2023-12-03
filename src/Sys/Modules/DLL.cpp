#include "DLL.hpp"
#include "Sys/Log.hpp"

#include <stdexcept>

namespace IW3SR
{
	DLL::DLL(std::string filePath)
	{
		FilePath = filePath;
		Instance = LoadLibrary(FilePath.c_str());

		if (!Instance)
			throw std::runtime_error("Couldn't load DLL.");

		CallbackInitialize = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "Initialize"));
		CallbackGUI = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "GUI"));
		CallbackShutdown = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "Shutdown"));

		if (!CallbackInitialize)
			throw std::runtime_error("DLL is missing Initialize function.");

		SetRenderer();
		CallbackInitialize(this);
	}

	DLL::~DLL()
	{
		if (CallbackShutdown)
			CallbackShutdown();

		FreeLibrary(Instance);
		Instance = nullptr;
	}

	void DLL::SetRenderer()
	{
		if (CallbackGUI)
			CallbackGUI();
	}

	void DLL::SetInfos(const std::string& id, const std::string& name)
	{
		ID = id;
		Name = name;
	}
}
