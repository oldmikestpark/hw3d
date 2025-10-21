#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include <memory>

namespace Bind
{
	class Bindable;
	class IndexBuffer;
}

class Drawable 
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& gfx) const noexcept;
	virtual ~Drawable() = default;
protected:
	template<class T>
	T* QueryBindable() noexcept 
	{
		for (auto& pd : binds) 
		{
			if (auto pt = dynamic_cast<T*>(pd.get())) 
			{
				return pt;
			}
		}
		return nullptr;
	}
	void AddBind(std::shared_ptr<Bind::Bindable> bind) noexcept;
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bind::Bindable>> binds;
};
