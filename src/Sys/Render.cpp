#include "Render.hpp"
#include "Game/Game.hpp"
#include "Utils/Math.hpp"

namespace IW3SR
{
	void Render::Frame()
	{
		
	}

	int Render::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec4 color, bool depthTest, 
		std::vector<GfxPointVertex> verts)
	{
		float colorArray[4];
		int vertsCount = verts.size();
		const int vertsLimit = 2725;

		if (vertsCount + 2 > vertsLimit)
		{
			RB_DrawLines3D(vertsCount / 2, 1, verts.data(), depthTest);
			vertsCount = 0;
		}
		glm::set_float4(colorArray, color);
		R_ConvertColorToBytes(colorArray, verts[vertsCount].color);
		*reinterpret_cast<int*>(verts[vertsCount + 1].color) = *reinterpret_cast<int*>(verts[vertsCount].color);

		glm::set_float3(verts[vertsCount].xyz, start);
		glm::set_float3(verts[vertsCount + 1].xyz, end);

		return vertsCount + 2;
	}
}
