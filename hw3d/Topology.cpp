#include "Topology.h"

namespace Bind
{
	Topology::Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type)
		:
		type(type)
	{}

	void Topology::Bind(Graphics& gfx) noexcept
	{
		GetContex(gfx)->IASetPrimitiveTopology(type);
	}
}