#include "Base.hpp"
#include "Texture.hpp"

#include "Engine/Core/Browser/Browser.hpp"

namespace IzEngine
{
	void BrowserClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
		const void* buffer, int width, int height)
	{
		if (!Renderer::Active || !Instance || !buffer || dirtyRects.empty())
			return;

		std::scoped_lock lock(Instance->TextureMutex);

		Instance->Texture = Texture::Create({ .ID = "browser_" + Instance->ID,
			.Size = Instance->FrameSize,
			.Usage = TextureUsage::Dynamic,
			.Pool = TexturePool::Default });

		auto dxTexture = std::static_pointer_cast<DX9Texture>(Instance->Texture);
		if (!dxTexture->Data)
			return;

		for (const auto& dirtyRect : dirtyRects)
		{
			D3DLOCKED_RECT lockedRect;
			RECT rect = { dirtyRect.x, dirtyRect.y, dirtyRect.x + dirtyRect.width, dirtyRect.y + dirtyRect.height };
			dxTexture->Data->LockRect(0, &lockedRect, &rect, 0);

			const uint8_t* src = reinterpret_cast<const uint8_t*>(buffer) + (dirtyRect.y * width + dirtyRect.x) * 4;
			uint8_t* dst = reinterpret_cast<uint8_t*>(lockedRect.pBits);

			for (int y = 0; y < dirtyRect.height; ++y)
			{
				std::memcpy(dst, src, dirtyRect.width * 4);
				src += width * 4;
				dst += lockedRect.Pitch;
			}
			dxTexture->Data->UnlockRect(0);
		}
	}

	void Browser::Kill()
	{
		auto processes = System::MapProcesses();
		HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, processes["CEF.exe"]);

		if (process)
		{
			TerminateProcess(process, 0);
			CloseHandle(process);
		}
	}
}
