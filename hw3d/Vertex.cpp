#include "Vertex.h"

using namespace Dvtx;

Dvtx::VertexLayout::Element::Element(ElementType type, size_t offset)
	:
	type(type),
	offset(offset)
{}

size_t Dvtx::VertexLayout::Element::GetOffsetAfter() const noexcept
{
	return offset + Size();
}

size_t Dvtx::VertexLayout::Element::GetOffset() const noexcept
{
	return offset;
}

size_t Dvtx::VertexLayout::Element::Size() const noexcept
{
	return SizeOf(type);
}

constexpr size_t Dvtx::VertexLayout::Element::SizeOf(ElementType type) noexcept
{
	switch (type)
	{
	case Position2D:
		return sizeof(Map<Position2D>::SysType);
	case Position3D:
		return sizeof(Map<Position3D>::SysType);
	case Texture2D:
		return sizeof(Map<Texture2D>::SysType);
	case Normal:
		return sizeof(Map<Normal>::SysType);
	case Float3Color:
		return sizeof(Map<Float3Color>::SysType);
	case Float4Color:
		return sizeof(Map<Float4Color>::SysType);
	case BGRAColor:
		return sizeof(Map<BGRAColor>::SysType);
	}
	assert("Invalid element type" && false);
	return 0u;
}

VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
{
	return type;
}

D3D11_INPUT_ELEMENT_DESC Dvtx::VertexLayout::Element::GetDesc() const noexcept
{
	switch (type)
	{
	case Position2D:
		return GenerateDesc<Position2D>(GetOffset());
	case Position3D:
		return GenerateDesc<Position3D>(GetOffset());
	case Texture2D:
		return GenerateDesc<Texture2D>(GetOffset());
	case Normal:
		return GenerateDesc<Normal>(GetOffset());
	case Float3Color:
		return GenerateDesc<Float3Color>(GetOffset());
	case Float4Color:
		return GenerateDesc<Float4Color>(GetOffset());
	case BGRAColor:
		return GenerateDesc<BGRAColor>(GetOffset());
	default:
		assert("Invalid element type" && false);
		return { "INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	}
}

const char* Dvtx::VertexLayout::Element::GetCode() const noexcept
{
	switch (type) 
	{
	case Position2D:
		return Map<Position2D>::code;
	case Position3D:
		return Map<Position3D>::code;
	case Texture2D:
		return Map<Texture2D>::code;
	case Normal:
		return Map<Normal>::code;
	case Float3Color:
		return Map<Float3Color>::code;
	case Float4Color:
		return Map<Float4Color>::code;
	case BGRAColor:
		return Map<BGRAColor>::code;
	}
	assert("Invalid element type" && false);
	return "Invalid";
}

const VertexLayout::Element& Dvtx::VertexLayout::ResolveByIndex(size_t i) const noexcept
{
	return elements[i];
}

VertexLayout& Dvtx::VertexLayout::Append(ElementType type) noexcept
{
	elements.emplace_back(type, Size());
	return *this;
}

size_t Dvtx::VertexLayout::Size() const noexcept
{
	return elements.empty() ? 0u : elements.back().GetOffsetAfter();
}

size_t Dvtx::VertexLayout::GetElementCount() const noexcept
{
	return elements.size();
}

std::vector<D3D11_INPUT_ELEMENT_DESC> Dvtx::VertexLayout::GetD3DLayout() const noexcept
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	for (const auto& e : elements)
	{
		desc.push_back(e.GetDesc());
	}
	return desc;
}

std::string Dvtx::VertexLayout::GetCode() const noexcept
{
	std::string code;
	for (const auto& e : elements) 
	{
		code += e.GetCode();
	}
	return code;
}

Dvtx::Vertex::Vertex(char* pData, const VertexLayout& layout)
	:
	pData(pData),
	layout(layout)
{}

Dvtx::ConstVertex::ConstVertex(const Vertex& v) noexcept
	:
	vertex(v)
{}

Dvtx::VertexBuffer::VertexBuffer(VertexLayout layout, size_t size) noexcept
	:
	layout(std::move(layout))
{
	Resize(size);
}

const char* Dvtx::VertexBuffer::GetData() const noexcept
{
	return buffer.data();
}

const VertexLayout& Dvtx::VertexBuffer::GetLayout() const noexcept
{
	return layout;
}

void Dvtx::VertexBuffer::Resize(size_t newSize) noexcept
{
	const auto size = Size();
	if (size < newSize) 
	{
		buffer.resize(buffer.size() + layout.Size() * (newSize - size));
	}
}

size_t Dvtx::VertexBuffer::Size() const noexcept
{
	return buffer.size() / layout.Size();
}

size_t Dvtx::VertexBuffer::SizeBytes() const noexcept
{
	return buffer.size();
}

Vertex Dvtx::VertexBuffer::Back() noexcept
{
	assert(buffer.size() != 0u);
	return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
}

Vertex Dvtx::VertexBuffer::Front() noexcept
{
	assert(buffer.size() != 0u);
	return Vertex{ buffer.data(), layout };
}

Vertex Dvtx::VertexBuffer::operator[](size_t i) noexcept
{
	assert(i < Size());
	return Vertex{ buffer.data() + layout.Size() * i, layout };
}

ConstVertex Dvtx::VertexBuffer::Back() const noexcept
{
	return const_cast<VertexBuffer*>(this)->Back();
}

ConstVertex Dvtx::VertexBuffer::Front() const noexcept
{
	return const_cast<VertexBuffer*>(this)->Front();
}

ConstVertex Dvtx::VertexBuffer::operator[](size_t i) const noexcept
{
	return const_cast<VertexBuffer&>(*this)[i];
}
