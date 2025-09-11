#pragma once
#include "DrawableBase.h"
#include "BindableCommant.h"
#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <optional>

using namespace Bind;

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bindable>> bindPtr);
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node 
{
	friend class Model;
	friend class ModelWindow;
public:
	Node(const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept;
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
	void SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept;
private:
	void AddChild(std::unique_ptr<Node> pChild) noexcept;
	void ShowTree(int& nodeIndexTracked, std::optional<int>& selectedIndex, Node*& pSelectedNode) const noexcept;
private:
	std::string name;
	std::vector<std::unique_ptr<Node>> childPtr;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT4X4 appliedTransform;
};

class Model 
{
public:
	Model(Graphics& gfx, const std::string fileName);
	void Draw(Graphics& gfx) const;
	void ShowWindow(const char* windowName = nullptr) noexcept;
	~Model() noexcept;
private:
	std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);
	std::unique_ptr<Node> ParseNode(const aiNode& node);
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
};
