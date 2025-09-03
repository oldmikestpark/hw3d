#pragma once
#include "TestObject.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

class Box : public TestObject<Box>
{
public:
	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT3 material
	);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow(int id, Graphics& gfx) noexcept;
private:
	void SyncMaterial(Graphics& gfx) noexcept;
private:
	struct PSMaterialConstant
	{
		dx::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float speculatPower = 30.0f;
		float padding[3];
	} materialConst;
	using MaterialCBuf = PixelConstantBuffer<PSMaterialConstant>;
private:
	// model transform
	DirectX::XMFLOAT3X3 mt;
};