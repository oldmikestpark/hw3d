#include "VertexShader.h"

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	}

	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		GetContex(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return pBlob.Get();
	}
}
