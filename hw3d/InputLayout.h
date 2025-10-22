#pragma once
#include "Bindable.h"
#include "Vertex.h"

namespace Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphics& gfx, 
			Dvtx::VertexLayout layout,
			ID3DBlob* pVertexShaderByteCode
		);
		static std::shared_ptr<Bindable> Resolve(
			Graphics& gfx,
			const Dvtx::VertexLayout& layout,
			ID3DBlob* pVertexShaderByteCode
		);
		static std::string GenerateUID(const Dvtx::VertexLayout layout, ID3DBlob* pVertexShaderByteCode = nullptr);
		std::string GetUID() const noexcept override;
		void Bind(Graphics& gfx) noexcept override;
	protected:
		Dvtx::VertexLayout layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}