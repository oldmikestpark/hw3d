#pragma once
#include "Bindable.h"

namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type);
		void Bind(Graphics& gfx) noexcept override;
	protected:
		D3D_PRIMITIVE_TOPOLOGY type;
	};
}