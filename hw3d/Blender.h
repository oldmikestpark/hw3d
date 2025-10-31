#pragma once
#include "Bindable.h"
#include <array>
#include <optional>

namespace Bind 
{
	class Blender : public Bindable
	{
	public:
		Blender(Graphics& gfx, bool blending, std::optional<float> factor = {});
		void Bind(Graphics& gfx) noexcept override;
		void SetFactor(float factor) noexcept;
		float GetFactor() const noexcept;
		static std::shared_ptr<Blender> Resolve(Graphics& gfx, bool blending, std::optional<float> = {});
		static std::string GenerateUID(bool blending, std::optional<float> = {});
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
		bool blending;
		std::optional<std::array<float, 4>> factors;
	};
}