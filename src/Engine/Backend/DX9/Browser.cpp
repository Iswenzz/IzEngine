#include "Base.hpp"
#include "Texture.hpp"

#include "Engine/Core/Browser/Browser.hpp"

namespace IzEngine
{
	void BrowserClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
		const void* buffer, int width, int height)
	{
		if (!buffer || dirtyRects.empty())
			return;

		std::scoped_lock lock(Browser::TextureMutex);
		Browser::Texture = Texture::Create({ .ID = "browser", .Size = Browser::Size });
		IDirect3DTexture9* texture = std::static_pointer_cast<DX9Texture>(Browser::Texture)->Data;

		for (const auto& dirtyRect : dirtyRects)
		{
			D3DLOCKED_RECT lockedRect;
			RECT rect = { dirtyRect.x, dirtyRect.y, dirtyRect.x + dirtyRect.width, dirtyRect.y + dirtyRect.height };
			texture->LockRect(0, &lockedRect, &rect, 0);

			const uint8_t* src = reinterpret_cast<const uint8_t*>(buffer) + (dirtyRect.y * width + dirtyRect.x) * 4;
			uint8_t* dst = reinterpret_cast<uint8_t*>(lockedRect.pBits);

			for (int y = 0; y < dirtyRect.height; ++y)
			{
				std::memcpy(dst, src, dirtyRect.width * 4);
				src += width * 4;
				dst += lockedRect.Pitch;
			}
			texture->UnlockRect(0);
		}
	}
}
