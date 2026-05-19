#include "Base.hpp"

#include "Engine/Core/System/AssetManager.hpp"

namespace IzEngine
{
	void AssetManager::LoadSystemFonts()
	{
		const auto callback = [](const LOGFONT* lpelf, const TEXTMETRIC*, DWORD, LPARAM) -> int
		{
			FontNames.push_back(lpelf->lfFaceName);
			return true;
		};
		HDC hdc = GetDC(nullptr);
		EnumFontFamilies(hdc, nullptr, static_cast<FONTENUMPROC>(callback), 0);
		ReleaseDC(nullptr, hdc);
	}
}
