#include "TexturePreprocessor.h"
#include <filesystem>
#include "Mesh.h"
#include "ChiliMath.h"

template<typename F>
inline void TexturePreprocessor::TransformFile(const std::string& pathIn, const std::string& pathOut, F&& func)
{
	auto surf = Surface::FromFile(pathIn);
	TransformSurface(surf, func);
	surf.Save(pathOut);
}

template<typename F>
inline static void TexturePreprocessor::TransformSurface(Surface& surf, F&& func) 
{
	const auto width = surf.GetWidth();
	const auto height = surf.GetHeight();
	for (unsigned int x = 0; x < width; x++) 
	{
		for (unsigned int y = 0; y < height; y++) 
		{
			const auto n = ColorToVector(surf.GetPixel(x, y));
			surf.PutPixel(x, y, VectorToColor(n));
		}
	}
}

void TexturePreprocessor::FlipYAllNormalMapsInObj(const std::string& objPath)
{
	const auto rootPath = std::filesystem::path{ objPath }.parent_path().string() + "\\";

	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(objPath, 0u);

	for (auto i = 0u; i < pScene->mNumMaterials; i++) 
	{
		const auto& mat = *pScene->mMaterials[i];
		aiString texFileName;
		if (mat.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS) 
		{
			const auto path = rootPath + texFileName.C_Str();
			FlipYNormalMap(path, path);
		}
	}
}

void TexturePreprocessor::FlipYNormalMap(const std::string& pathIn, const std::string& pathOut)
{
	using namespace DirectX;
	const auto flipY = XMVectorSet(1.0f, -1.0f, 1.0f, 1.0f);
	const auto ProcessNormal = [flipY](FXMVECTOR n, int x, int y) -> XMVECTOR
	{
		return XMVectorMultiply(flipY, n);
	};

	TransformFile(pathIn, pathOut, ProcessNormal);
}

void TexturePreprocessor::ValidateNormalMap(const std::string& pathIn, float thresholdMin, float thresholdMax)
{
	OutputDebugStringA(("Validate normal map [" + pathIn + "]\n").c_str());

	using namespace DirectX;
	auto sum = XMVectorZero();
	const auto ProcessNormal = [thresholdMin, thresholdMax, &sum](FXMVECTOR n, int x, int y) 
	{
		const float len = XMVectorGetX(XMVector3Length(n));
		const float z = XMVectorGetZ(n);
		if (len < thresholdMin || len > thresholdMax) 
		{
			XMFLOAT3 vec;
			XMStoreFloat3(&vec, n);
			std::ostringstream oss;
			oss << "Bad normal length: " << len << " at: (" << x << ", " << y << ") normal: (" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
			OutputDebugStringA(oss.str().c_str());
		}
		if (z < 0.0f) 
		{
			XMFLOAT3 vec;
			XMStoreFloat3(&vec, n);
			std::ostringstream oss;
			oss << "Bad normal Z direction at : (" << x << ", " << y << ") normal: (" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
			OutputDebugStringA(oss.str().c_str());
		}
		sum = XMVectorAdd(sum, n);
		return n;
	};
	auto surf = Surface::FromFile(pathIn);
	TransformSurface(surf, ProcessNormal);
	{
		XMFLOAT2 sumv;
		XMStoreFloat2(&sumv, sum);
		std::ostringstream oss;
		oss << "Normal map biases: (" << sumv.x << ", " << sumv.y << ")\n";
		OutputDebugStringA(oss.str().c_str());
	}
}

DirectX::XMVECTOR TexturePreprocessor::ColorToVector(Surface::Color c) noexcept
{
	using namespace DirectX;
	auto n = XMVectorSet((float)c.GetR(), (float)c.GetG(), (float)c.GetB(), 0.0f);
	const auto all255 = XMVectorReplicate(2.0f / 255.0f);
	n = XMVectorMultiply(n, all255);
	const auto all1 = XMVectorReplicate(1.0f);
	n = XMVectorSubtract(n, all1);
	return n;
}

Surface::Color TexturePreprocessor::VectorToColor(DirectX::XMVECTOR n) noexcept
{
	using namespace DirectX;
	const auto all1 = XMVectorReplicate(1.0f);
	XMVECTOR nOut = XMVectorAdd(n, all1);
	const auto all255 = XMVectorReplicate(255.0f / 2.0f);
	nOut = XMVectorMultiply(nOut, all255);
	XMFLOAT3 floats;
	XMStoreFloat3(&floats, nOut);
	return {
		(unsigned char)round(floats.x),
		(unsigned char)round(floats.y),
		(unsigned char)round(floats.z),
	};
}
