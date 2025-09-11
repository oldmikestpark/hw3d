#include "Mesh.h"
#include "imgui\imgui.h"

Mesh::Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bindable>> bindPtr)
{
	if (!IsStaticInitialized())
	{
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	for (auto& pb : bindPtr)
	{
		if (auto pi = dynamic_cast<IndexBuffer*>(pb.get()))
		{
			AddIndexBuffer(std::unique_ptr<IndexBuffer>{pi});
			pb.release();
		}
		else
		{
			AddBind(std::move(pb));
		}
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Draw(gfx);
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}

Node::Node(const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept
	:
	meshPtrs(meshPtrs),
	name(name)
{
	DirectX::XMStoreFloat4x4(&this->transform, transform);
}

void Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept
{
	const auto built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;

	for (const auto pm : meshPtrs)
	{
		pm->Draw(gfx, built);
	}

	for (const auto& pc : childPtr)
	{
		pc->Draw(gfx, built);
	}
}

void Node::ShowTree() const noexcept
{
	// if tree node expanded, recursively render all children
	if (ImGui::TreeNode(name.c_str())) 
	{
		for (const auto& pChild : childPtr) 
		{
			pChild->ShowTree();
		}
		ImGui::TreePop();
	}
}

void Node::AddChild(std::unique_ptr<Node> pChild) noexcept
{
	assert(pChild);
	childPtr.push_back(std::move(pChild));
}

class ModelWindow 
{
public:
	void Show(const char* windowName, const Node& root) noexcept
	{
		windowName = windowName ? windowName : "Model";
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree();

			ImGui::NextColumn();
			ImGui::Text("Orientation");
			ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
			ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

			ImGui::Text("Position");
			ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
			ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
			ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);

			if (ImGui::Button("Reset"))
			{
				Reset();
			}
		}
		ImGui::End();
	}
	DirectX::XMMATRIX GetTransform() const noexcept 
	{
		return DirectX::XMMatrixRotationRollPitchYaw(pos.pitch, pos.yaw, pos.roll) *
			DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	}
private:
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
	void Reset() noexcept 
	{
		pos.pitch = 0.0f;
		pos.yaw = 0.0f;
		pos.roll = 0.0f;
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;
	}
};

Model::Model(Graphics& gfx, const std::string fileName)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);

	for (size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i]));
	}

	pRoot = ParseNode(*pScene->mRootNode);
}

void Model::Draw(Graphics& gfx) const
{
	pRoot->Draw(gfx, pWindow->GetTransform());
}

void Model::ShowWindow(const char* windowName) noexcept
{
	pWindow->Show(windowName, *pRoot);
}

Model::~Model() noexcept
{}

std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh)
{
	namespace dx = DirectX;
	using Dvtx::VertexLayout;

	Dvtx::VertexBuffer vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)
	));

	for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
	{
		vbuf.EmplaceBack(
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; ++i)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	std::vector<std::unique_ptr<Bindable>> bindablePtrs;

	bindablePtrs.push_back(std::make_unique<VertexBuffer>(gfx, vbuf));
	bindablePtrs.push_back(std::make_unique<IndexBuffer>(gfx, indices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
	auto pvsbc = pvs->GetBytecode();
	bindablePtrs.push_back(std::move(pvs));

	bindablePtrs.push_back(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));
	bindablePtrs.push_back(std::make_unique<InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color = { 0.6f, 0.6f, 0.8f };
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	}pmc;

	bindablePtrs.push_back(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));

	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

std::unique_ptr<Node> Model::ParseNode(const aiNode& node)
{
	namespace dx = DirectX;

	const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	));

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; ++i)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; ++i)
	{
		pNode->AddChild(ParseNode(*node.mChildren[i]));
	}

	return pNode;
}
