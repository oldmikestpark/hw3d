#pragma once
#include <vector>
#include <DirectXMath.h>
#include <unordered_map>
#include <optional>
#include <assert.h>
#include <memory>
#include <type_traits>

#define RESOLVE_BASE(eltype)\
virtual size_t Resolve ## eltype() const noexcept\
{\
	assert(false && "Can not resolve to " #eltype); return 0u;\
}

#define LEAF_ELEMENT(eltype, systype)\
class eltype : public LayoutElement\
{\
public:\
	using SystemType = systype;\
	using LayoutElement::LayoutElement;\
	size_t Resolve ## eltype() const noexcept override final\
	{\
		return GetOffsetBegin();\
	}\
	size_t GetOffsetEnd() const noexcept override final\
	{\
		return GetOffsetBegin() + sizeof(systype);\
	}\
};

#define REF_CONVERSION(eltype)\
operator eltype::SystemType&() noexcept\
{\
	return *reinterpret_cast<eltype::SystemType*>(pBytes + offset + pLayout->Resolve ## eltype());\
}\
eltype::SystemType& operator=(const eltype::SystemType& rhs) noexcept\
{\
	return static_cast<eltype::SystemType&>(*this) = rhs;\
}

namespace Dcb 
{
	class Struct;
	class Array;

	namespace dx = DirectX;
	class LayoutElement 
	{
	public:
		LayoutElement(size_t offset)
			:
			offset(offset) 
		{}
		virtual LayoutElement& operator[](const char*) 
		{
			assert(false && "cannot access member on non struct");
			return *this;
		}
		virtual const LayoutElement& operator[](const char* key) const 
		{
			assert(false && "cannot access member on non struct");
			return *this;
		}
		virtual LayoutElement& T() 
		{
			assert(false);
			return *this;
		}
		virtual const LayoutElement& T() const 
		{
			assert(false);
			return *this;
		}
		size_t GetOffsetBegin() const noexcept
		{
			return offset;
		}
		virtual size_t GetOffsetEnd() const noexcept = 0;

		size_t GetSizeInBytes() const noexcept 
		{
			return GetOffsetEnd() - GetOffsetBegin();
		}

		template<typename U>
		Struct& Add(const char* key) noexcept;
		template<typename U>
		Array& Set(size_t size) noexcept;

		RESOLVE_BASE(Matrix)
		RESOLVE_BASE(Float4)
		RESOLVE_BASE(Float3)
		RESOLVE_BASE(Float2)
		RESOLVE_BASE(Float)
		RESOLVE_BASE(Bool)
	private:
		size_t offset;
	};

	LEAF_ELEMENT(Matrix, dx::XMFLOAT4X4)
	LEAF_ELEMENT(Float4, dx::XMFLOAT4)
	LEAF_ELEMENT(Float3, dx::XMFLOAT3)
	LEAF_ELEMENT(Float2, dx::XMFLOAT2)
	LEAF_ELEMENT(Float, float)
	LEAF_ELEMENT(Bool, BOOL)

	class Struct : public LayoutElement 
	{
	public:
		using LayoutElement::LayoutElement;
		LayoutElement& operator[](const char* key) override final
		{
			return *map.at(key);
		}
		const LayoutElement& operator[](const char* key) const override final 
		{
			return *map.at(key);
		}
		size_t GetOffsetEnd() const noexcept override final 
		{
			return elements.empty() ? GetOffsetBegin() : elements.back()->GetOffsetEnd();
		}
		template<typename U>
		Struct& Add(const char* name) noexcept
		{
			elements.push_back(std::make_unique<U>(GetOffsetEnd()));
			if (!map.emplace(name, elements.back().get()).second) 
			{
				assert(false);
			}
			return *this;
		}
	private:
		std::unordered_map<std::string, LayoutElement*> map;
		std::vector<std::unique_ptr<LayoutElement>> elements;
	};

	class Array : public LayoutElement
	{
	public:
		using LayoutElement::LayoutElement;
		size_t GetOffsetEnd() const noexcept override final 
		{
			assert(pElement);
			return GetOffsetBegin() + pElement->GetSizeInBytes() * size;
		}
		template<typename U>
		Array& Set(size_t size_in) noexcept
		{
			pElement = std::make_unique<U>(GetOffsetBegin());
			size = size_in;
			return *this;
		}
		LayoutElement& T() override final
		{
			return *pElement;
		}
		const LayoutElement& T() const override final 
		{
			return *pElement;
		}
	private:
		size_t size{0u};
		std::unique_ptr<LayoutElement> pElement;
	};

	class ElementRef 
	{
	public:
		ElementRef(const LayoutElement* pLayout, char* pBytes, size_t offset)
			:
			pLayout(pLayout),
			pBytes(pBytes),
			offset(offset)
		{}
		ElementRef operator[](const char* key) noexcept 
		{
			return {&(*pLayout)[key], pBytes, offset};
		}
		ElementRef operator[](const size_t index) noexcept 
		{
			const auto& t{pLayout->T()};
			return {&t, pBytes, offset + t.GetSizeInBytes() * index};
		}

		REF_CONVERSION(Matrix)
		REF_CONVERSION(Float4)
		REF_CONVERSION(Float3)
		REF_CONVERSION(Float2)
		REF_CONVERSION(Float)
		REF_CONVERSION(Bool)
	private:
		size_t offset;
		const class LayoutElement* pLayout;
		char* pBytes;
	};

	class Buffer 
	{
	public:
		Buffer(std::shared_ptr<Struct> pLayout)
			:
			pLayout(pLayout),
			bytes(pLayout->GetOffsetEnd()) 
		{}
		ElementRef operator[](const char* key) noexcept 
		{
			return {&(*pLayout)[key], bytes.data(), 0u};
		}
		const char* GetData() const noexcept 
		{
			return bytes.data();
		}
		size_t GetSizeInBytes() const noexcept 
		{
			return bytes.size();
		}
		const LayoutElement& GetLayout() const noexcept 
		{
			return *pLayout;
		}
		std::shared_ptr<LayoutElement> CloneLayout() const 
		{
			return pLayout;
		}
	private:
		std::shared_ptr<Struct> pLayout;
		std::vector<char> bytes;
	};

	template<typename U>
	Struct& LayoutElement::Add(const char* key) noexcept 
	{
		auto ps{dynamic_cast<Struct*>(this)};
		assert(ps != nullptr);
		return ps->Add<U>(key);
	}
	template<typename U>
	Array& LayoutElement::Set(size_t size) noexcept 
	{
		auto pa{dynamic_cast<Array*>(this)};
		assert(pa != nullptr);
		return pa->Set<U>(size);
	}
}