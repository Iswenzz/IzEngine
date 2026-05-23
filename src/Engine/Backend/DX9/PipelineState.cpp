#include "PipelineState.hpp"
#include "GraphicsContext.hpp"

namespace IzEngine
{
	static D3DCULL GetCullMode(CullMode mode)
	{
		switch (mode)
		{
		case CullMode::None:
			return D3DCULL_NONE;
		case CullMode::Front:
			return D3DCULL_CW;
		case CullMode::Back:
			return D3DCULL_CCW;
		}
		return D3DCULL_NONE;
	}

	static void GetBlendMode(BlendMode mode, D3DBLEND& src, D3DBLEND& dst)
	{
		switch (mode)
		{
		case BlendMode::Alpha:
			src = D3DBLEND_SRCALPHA;
			dst = D3DBLEND_INVSRCALPHA;
			break;
		case BlendMode::Additive:
			src = D3DBLEND_SRCALPHA;
			dst = D3DBLEND_ONE;
			break;
		case BlendMode::Multiply:
			src = D3DBLEND_DESTCOLOR;
			dst = D3DBLEND_ZERO;
			break;
		default:
			src = D3DBLEND_ONE;
			dst = D3DBLEND_ZERO;
			break;
		}
	}

	DX9PipelineState::DX9PipelineState(const PipelineStateSpecification& spec)
	{
		Spec = spec;
	}

	void DX9PipelineState::Bind()
	{
		// Depth
		DX9GraphicsContext::Device->SetRenderState(D3DRS_ZENABLE, Spec.DepthTest ? D3DZB_TRUE : D3DZB_FALSE);
		DX9GraphicsContext::Device->SetRenderState(D3DRS_ZWRITEENABLE, Spec.DepthWrite);

		// Culling
		DX9GraphicsContext::Device->SetRenderState(D3DRS_CULLMODE, GetCullMode(Spec.Cull));

		// Blend
		DX9GraphicsContext::Device->SetRenderState(D3DRS_ALPHABLENDENABLE,
			Spec.Blend != BlendMode::None ? TRUE : FALSE);

		if (Spec.Blend != BlendMode::None)
		{
			D3DBLEND src, dst;
			GetBlendMode(Spec.Blend, src, dst);
			DX9GraphicsContext::Device->SetRenderState(D3DRS_SRCBLEND, src);
			DX9GraphicsContext::Device->SetRenderState(D3DRS_DESTBLEND, dst);
		}
		// Misc
		DX9GraphicsContext::Device->SetRenderState(D3DRS_LIGHTING, 0);
		DX9GraphicsContext::Device->SetRenderState(D3DRS_FOGENABLE, Spec.Fog);
		DX9GraphicsContext::Device->SetRenderState(D3DRS_STENCILENABLE, Spec.Stencil);
		DX9GraphicsContext::Device->SetRenderState(D3DRS_SCISSORTESTENABLE, Spec.Scissor);
		DX9GraphicsContext::Device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
		DX9GraphicsContext::Device->SetRenderState(D3DRS_ALPHATESTENABLE, 0);

		// Fill mode
		DX9GraphicsContext::Device->SetRenderState(D3DRS_FILLMODE,
			Spec.Fill == FillMode::Wireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	}
}
