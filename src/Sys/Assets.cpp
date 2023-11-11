#include "Assets.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	void Assets::Initialize() { }

	void Assets::Shutdown()
	{
		for (const auto& [name, image] : Images)
			image->Release();
	}

	IDirect3DTexture9* Assets::LoadTexture(const std::string& filePath)
	{
		if (!std::filesystem::exists(filePath))
			throw std::runtime_error("Couldn't find texture path.");

		IDirect3DTexture9* texture = nullptr;
		D3DXCreateTextureFromFile(dx->device, filePath.c_str(), &texture);

		Images.insert({ filePath, texture });
		return texture;
	}
}