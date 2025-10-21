#include "Drawable.h"
#include "IndexBuffer.h"
#include "GraphicsThrowMacros.h"

using namespace Bind;

void Drawable::Draw(Graphics& gfx) const noexcept
{
	for (auto& b : binds) 
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind) noexcept
{
	// special case for index buffer
	if (typeid(*bind) == typeid(IndexBuffer)) 
	{
		assert("Binding multiple index buffers is not allowed" && pIndexBuffer == nullptr);
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	binds.push_back(std::move(bind));
}
