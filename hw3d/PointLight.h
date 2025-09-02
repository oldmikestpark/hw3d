#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffer.h"

class PointLight 
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept;
	void Bind(Graphics& gfx) const noexcept;
private:
	struct PointLightCBuf 
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 materialColor;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuseColor;
		float diffuseIndensity;
		float attConst;
		float attLin;
		float attQuau;
	};
private:
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};