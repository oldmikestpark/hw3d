#include "VertexShader.h"
#include "BindableCodex.h"
#include <typeinfo>

namespace Bind
{
	using namespace std::string_literals;

	VertexShader::VertexShader(Graphics& gfx, const std::string& path)
		:
		path(path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{path.begin(), path.end()}.c_str(), &pBlob));
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
	std::shared_ptr<Bindable> VertexShader::Resolve(Graphics& gfx, std::string& path)
	{
		auto bind = Codex::Resolve(GenerateGUI(path));
		if (!bind) 
		{
			bind = std::make_shared<VertexShader>(gfx, path);
			Codex::Store(bind);
		}
		return bind;
	}
	std::string VertexShader::GenerateGUI(const std::string& path)
	{
		return typeid(VertexShader).name() + "#"s + path;
	}
	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateGUI(path);
	}
}
