#include "Rasterizer.h"
#include "BindableCodex.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	Bind::Rasterizer::Rasterizer(Graphics& gfx, bool twoSided)
		:
		twoSided(twoSided)
	{
		INFOMAN(gfx);

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
	}

	void Bind::Rasterizer::Bind(Graphics& gfx) noexcept
	{
		GetContex(gfx)->RSSetState(pRasterizer.Get());
	}

	std::shared_ptr<Rasterizer> Bind::Rasterizer::Resolve(Graphics& gfx, bool twoSided)
	{
		return Codex::Resolve<Rasterizer>(gfx, twoSided);
	}

	std::string Bind::Rasterizer::GenerateUID(bool twoSided)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
	}

	std::string Bind::Rasterizer::GetUID() const noexcept
	{
		return GenerateUID(twoSided);
	}
}