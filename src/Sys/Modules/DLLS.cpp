#include "DLLS.hpp"
#include <TlHelp32.h>

namespace IW3SR
{
	DLLS::~DLLS()
	{
		Modules.clear();
	}

	void DLLS::Initialize()
	{
		if (!std::filesystem::is_directory(Environment::PluginsDirectory))
			return;

		for (const auto& entry : std::filesystem::directory_iterator(Environment::PluginsDirectory))
		{
			if (entry.path().extension() == ".dll")
				Modules.insert({ entry.path().filename().string(), std::make_unique<DLL>(entry.path().string()) });
		}
	}

	void DLLS::Reload()
	{
		if (IsReloading) return;
		IsReloading = true;

		Modules.clear();
		std::thread([this] { Compile(); }).detach();
	}

	void DLLS::Compile()
	{
		if (std::filesystem::exists(CMAKE_BINARY_DIR))
		{
			constexpr auto command = R"(cd "{}" && cmake --build . --config Debug --target Install)";
			system(std::format(command, CMAKE_BINARY_DIR).c_str());
		}
		Initialize();
		IsReloading = false;
	}

	void DLLS::SetRenderer()
	{
		for (const auto& [_, dll] : Modules)
			dll->SetRenderer();
	}
}
