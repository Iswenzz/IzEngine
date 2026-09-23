#include "Base.hpp"
#include "Texture.hpp"

#include "Engine/Core/Browser/Browser.hpp"

namespace IzEngine
{
	void BrowserClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
		const void* buffer, int width, int height)
	{
		// A popup such as an open <select> paints in its own coordinates, and would land in the corner.
		if (type != PET_VIEW)
			return;
		if (!Renderer::Active || !Instance || !buffer || dirtyRects.empty() || Browser::Paused)
			return;

		std::scoped_lock lock(Instance->TextureMutex);

		if (!Renderer::Active || Browser::Paused)
			return;

		auto dxTexture = std::static_pointer_cast<DX9Texture>(Instance->Texture);
		if (!dxTexture || !dxTexture->Data || dxTexture->Spec.Usage != TextureUsage::Dynamic)
			return;

		for (const auto& dirtyRect : dirtyRects)
		{
			D3DLOCKED_RECT lockedRect = {};
			RECT rect = { dirtyRect.x, dirtyRect.y, dirtyRect.x + dirtyRect.width, dirtyRect.y + dirtyRect.height };
			if (FAILED(dxTexture->Data->LockRect(0, &lockedRect, &rect, 0)) || !lockedRect.pBits)
				continue;

			const uint32_t* src = reinterpret_cast<const uint32_t*>(buffer) + dirtyRect.y * width + dirtyRect.x;
			uint8_t* dst = reinterpret_cast<uint8_t*>(lockedRect.pBits);

			for (int y = 0; y < dirtyRect.height; ++y)
			{
				uint32_t* row = reinterpret_cast<uint32_t*>(dst);
				for (int x = 0; x < dirtyRect.width; ++x)
					row[x] = src[x] | 0xFF000000;

				src += width;
				dst += lockedRect.Pitch;
			}
			dxTexture->Data->UnlockRect(0);
		}
	}
}
