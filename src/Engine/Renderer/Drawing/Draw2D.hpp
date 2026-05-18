#pragma once
#include "Engine/Core/Common.hpp"

#include "Engine/Renderer/Camera/Camera.hpp"
#include "Engine/Renderer/Resources/Font.hpp"
#include "Engine/Renderer/Resources/Shader.hpp"
#include "Engine/Renderer/Resources/Texture.hpp"

namespace IzEngine
{
	struct Draw2DStatistics
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t LineCount = 0;

		uint32_t GetTotalVertexCount() const
		{
			return QuadCount * 4 + LineCount * 2;
		}

		uint32_t GetTotalIndexCount() const
		{
			return QuadCount * 6;
		}
	};

	class API Draw2D
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void Begin(const Camera& camera);
		static void End();

		static void DrawQuad(const vec3& position, const vec2& size, const vec4& color);
		static void DrawQuad(const vec3& position, const vec2& size, const Ref<Texture>& texture, const vec4& color);
		static void DrawQuad(const vec3& position, const vec2& size, float rotation, const vec4& color);
		static void DrawQuad(const vec3& position, const vec2& size, float rotation, const Ref<Texture>& texture,
			const vec4& color);

		static void DrawLine(const vec3& start, const vec3& end, const vec4& color);
		static void DrawRect(const vec3& position, const vec2& size, const vec4& color);
		static void DrawText(const std::string& text, const Ref<Font>& font, const vec3& position, float size,
			const vec4& color, const vec2& skew);
		static vec2 GetTextSize(const std::string& text, const Ref<Font>& font);

		static Draw2DStatistics GetStatistics();
		static void ResetStatistics();

	private:
		static void Flush();
		static void FlushQuads();
		static void FlushLines();
		static void StartBatch();
		static void NextBatch();
	};
}
