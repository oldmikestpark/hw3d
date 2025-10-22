#include "InputLayout.h"
#include "BindableCodex.h"

namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx, Dvtx::VertexLayout layout_in, ID3DBlob* pVertexShaderByteCode)
		:
		layout(std::move(layout_in))
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
			layout.GetD3DLayout().data(), (UINT)layout.GetD3DLayout().size(),
			pVertexShaderByteCode->GetBufferPointer(),
			pVertexShaderByteCode->GetBufferSize(),
			&pInputLayout
		));

	}

	std::shared_ptr<Bindable> InputLayout::Resolve(Graphics& gfx, 
		const Dvtx::VertexLayout& layout, 
		ID3DBlob* pVertexShaderByteCode)
	{
		return Codex::Resolve<InputLayout>(gfx, layout, pVertexShaderByteCode);
	}

	std::string InputLayout::GenerateUID(const Dvtx::VertexLayout layout, ID3DBlob* pVertexShaderByteCode)
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.GetCode();
	}

	std::string InputLayout::GetUID() const noexcept
	{
		return GenerateUID(layout);
	}

	void InputLayout::Bind(Graphics& gfx) noexcept
	{
		GetContex(gfx)->IASetInputLayout(pInputLayout.Get());
	}
}