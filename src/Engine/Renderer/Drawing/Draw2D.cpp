#include "Draw2D.hpp"

#include "Engine/Core/System/AssetManager.hpp"
#include "Engine/Renderer/Base/Buffer.hpp"
#include "Engine/Renderer/Base/PipelineState.hpp"
#include "Engine/Renderer/Base/RenderCommand.hpp"
#include "Engine/Renderer/Base/VertexArray.hpp"

namespace IzEngine
{
	static constexpr uint32_t MaxQuads = 10000;
	static constexpr uint32_t MaxQuadVertices = MaxQuads * 4;
	static constexpr uint32_t MaxQuadIndices = MaxQuads * 6;
	static constexpr uint32_t MaxLines = 5000;
	static constexpr uint32_t MaxLineVertices = MaxLines * 2;
	static constexpr uint32_t MaxTextureSlots = 16;

	struct QuadVertex
	{
		vec3 Position;
		vec4 Color;
		vec2 TexCoord;
		float TexIndex;
	};

	struct LineVertex
	{
		vec3 Position;
		vec4 Color;
	};

	struct Draw2DData
	{
		Ref<PipelineState> Pipeline;
		mat4 ViewProjection = mat4(1.0f);
		Draw2DStatistics Stats;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		vec4 QuadVertexPositions[4] = {};

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;
	};

	static Draw2DData Data;

	void Draw2D::Initialize()
	{
		Data.Pipeline = PipelineState::Default2D();

		// Quad
		Data.QuadVertexArray = VertexArray::Create();
		Data.QuadVertexBuffer = VertexBuffer::Create(MaxQuadVertices * sizeof(QuadVertex));
		Data.QuadVertexBuffer->SetLayout({ { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float4, "COLOR" },
			{ ShaderDataType::Float2, "TEXCOORD" }, { ShaderDataType::Float, "TEXCOORD" } });
		Data.QuadVertexArray->AddVertexBuffer(Data.QuadVertexBuffer);
		Data.QuadVertexBufferBase = new QuadVertex[MaxQuadVertices];

		uint32_t* quadIndices = new uint32_t[MaxQuadIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < MaxQuadIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}
		Data.QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(quadIndices, MaxQuadIndices));
		delete[] quadIndices;

		// Line
		Data.LineVertexArray = VertexArray::Create();
		Data.LineVertexBuffer = VertexBuffer::Create(MaxLineVertices * sizeof(LineVertex));
		Data.LineVertexBuffer->SetLayout(
			{ { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float4, "COLOR" } });
		Data.LineVertexArray->AddVertexBuffer(Data.LineVertexBuffer);
		Data.LineVertexBufferBase = new LineVertex[MaxLineVertices];

		// Black texture
		Data.TextureSlots[0] = AssetManager::Get<Texture>(TEXTURE_WHITE);

		// Unit quad positions — top-left origin
		Data.QuadVertexPositions[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Data.QuadVertexPositions[1] = { 1.0f, 0.0f, 0.0f, 1.0f };
		Data.QuadVertexPositions[2] = { 1.0f, 1.0f, 0.0f, 1.0f };
		Data.QuadVertexPositions[3] = { 0.0f, 1.0f, 0.0f, 1.0f };

		// Shaders
		Data.QuadShader = AssetManager::Get<Shader>(SHADER_QUAD);
		Data.LineShader = AssetManager::Get<Shader>(SHADER_LINE);
	}

	void Draw2D::Shutdown()
	{
		delete[] Data.QuadVertexBufferBase;
		delete[] Data.LineVertexBufferBase;

		Data.QuadVertexBufferBase = nullptr;
		Data.LineVertexBufferBase = nullptr;

		Data.QuadVertexArray = nullptr;
		Data.QuadVertexBuffer = nullptr;
		Data.QuadShader = nullptr;

		Data.LineVertexArray = nullptr;
		Data.LineVertexBuffer = nullptr;
		Data.LineShader = nullptr;

		for (auto& slot : Data.TextureSlots)
			slot = nullptr;

		Data.QuadIndexCount = 0;
		Data.LineVertexCount = 0;
		Data.TextureSlotIndex = 1;
	}

	void Draw2D::Begin(const Camera& camera)
	{
		Data.ViewProjection = camera.GetViewProjection();
		StartBatch();
	}

	void Draw2D::End()
	{
		Flush();
	}

	void Draw2D::StartBatch()
	{
		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;

		Data.LineVertexCount = 0;
		Data.LineVertexBufferPtr = Data.LineVertexBufferBase;

		Data.TextureSlotIndex = 1;
	}

	void Draw2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Draw2D::Flush()
	{
		Data.Pipeline->Bind();

		FlushQuads();
		FlushLines();
	}

	void Draw2D::FlushQuads()
	{
		if (!Data.QuadIndexCount)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)Data.QuadVertexBufferPtr - (uint8_t*)Data.QuadVertexBufferBase);
		Data.QuadVertexBuffer->SetData(Data.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < Data.TextureSlotIndex; i++)
		{
			if (Data.TextureSlots[i])
				Data.TextureSlots[i]->Bind(i);
		}
		Data.QuadShader->Bind();
		Data.QuadShader->SetMat4("ViewProjection", Data.ViewProjection);
		RenderCommand::DrawIndexed(Data.QuadVertexArray, Data.QuadIndexCount);
		Data.Stats.DrawCalls++;
	}

	void Draw2D::FlushLines()
	{
		if (!Data.LineVertexCount)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)Data.LineVertexBufferPtr - (uint8_t*)Data.LineVertexBufferBase);
		Data.LineVertexBuffer->SetData(Data.LineVertexBufferBase, dataSize);

		Data.LineShader->Bind();
		Data.LineShader->SetMat4("ViewProjection", Data.ViewProjection);
		RenderCommand::DrawLines(Data.LineVertexArray, Data.LineVertexCount);
		Data.Stats.DrawCalls++;
	}

	void Draw2D::DrawQuad(const vec3& position, const vec2& size, const vec4& color)
	{
		DrawQuad(position, size, Data.TextureSlots[0], color);
	}

	void Draw2D::DrawQuad(const vec3& position, const vec2& size, const Ref<Texture>& texture, const vec4& color)
	{
		if (Data.QuadIndexCount >= MaxQuadIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (Data.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (Data.TextureSlotIndex >= MaxTextureSlots)
				NextBatch();

			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex++] = texture;
		}
		mat4 transform = glm::translate(mat4(1.0f), position) * glm::scale(mat4(1.0f), { size.x, size.y, 1.0f });
		constexpr vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;
			Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;
		}
		Data.QuadIndexCount += 6;
		Data.Stats.QuadCount++;
	}

	void Draw2D::DrawQuad(const vec3& position, const vec2& size, float rotation, const vec4& color)
	{
		DrawQuad(position, size, rotation, Data.TextureSlots[0], color);
	}

	void Draw2D::DrawQuad(const vec3& position, const vec2& size, float rotation, const Ref<Texture>& texture,
		const vec4& color)
	{
		if (Data.QuadIndexCount >= MaxQuadIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (Data.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (Data.TextureSlotIndex >= MaxTextureSlots)
				NextBatch();

			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex++] = texture;
		}
		vec3 center = { position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z };

		mat4 transform = glm::translate(mat4(1.0f), center)
			* glm::rotate(mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::translate(mat4(1.0f), { -size.x * 0.5f, -size.y * 0.5f, 0.0f })
			* glm::scale(mat4(1.0f), { size.x, size.y, 1.0f });

		constexpr vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;
			Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;
		}
		Data.QuadIndexCount += 6;
		Data.Stats.QuadCount++;
	}

	void Draw2D::DrawLine(const vec3& start, const vec3& end, const vec4& color)
	{
		if (Data.LineVertexCount >= MaxLineVertices)
			NextBatch();

		Data.LineVertexBufferPtr->Position = start;
		Data.LineVertexBufferPtr->Color = color;
		Data.LineVertexBufferPtr++;

		Data.LineVertexBufferPtr->Position = end;
		Data.LineVertexBufferPtr->Color = color;
		Data.LineVertexBufferPtr++;

		Data.LineVertexCount += 2;
		Data.Stats.LineCount++;
	}

	void Draw2D::DrawRect(const vec3& position, const vec2& size, const vec4& color)
	{
		vec3 tl = { position.x, position.y, position.z };
		vec3 tr = { position.x + size.x, position.y, position.z };
		vec3 br = { position.x + size.x, position.y + size.y, position.z };
		vec3 bl = { position.x, position.y + size.y, position.z };

		DrawLine(tl, tr, color);
		DrawLine(tr, br, color);
		DrawLine(br, bl, color);
		DrawLine(bl, tl, color);
	}

	vec2 Draw2D::GetTextSize(const std::string& text, const Ref<Font>& font)
	{
		return font->GetTextSize(text);
	}

	Draw2DStatistics Draw2D::GetStatistics()
	{
		return Data.Stats;
	}

	void Draw2D::ResetStatistics()
	{
		Data.Stats = {};
	}
}
