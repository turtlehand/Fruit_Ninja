#include "pch.h"
#include "MeshManager.h"
#include "Mesh.h"

CMeshManager::CMeshManager()
{

}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::Init()
{
	// ColorMesh 사각형 생성 ============================================
	FVertexColor	CenterRectColor[4] =
	{
		FVertexColor(-0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 1.f)
	};

	unsigned short	CenterRectColorIdx[6] = { 0, 1, 3, 0, 3, 2 };

	if (!CreateMesh("Mesh_CenterRectColor", CenterRectColor, sizeof(FVertexColor),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterRectColorIdx, 2, 6, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;

	// TextureMesh 사각형 생성 ============================================
	FVertexTex	CenterRectTexture[4] =
	{
		FVertexTex(-0.5f, 0.5f, 0.f, 0.f, 0.f),
		FVertexTex(0.5f, 0.5f, 0.f, 1.f, 0.f),
		FVertexTex(-0.5f, -0.5f, 0.f, 0.f, 1.f),
		FVertexTex(0.5f, -0.5f, 0.f, 1.f, 1.f)
	};

	if (!CreateMesh("Mesh_CenterRectTex", CenterRectTexture,
		sizeof(FVertexTex),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterRectColorIdx, 2, 6, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;

	// CenterCubeColor 큐브 생성 ============================================
	FVertexColor	CenterCubeColor[8] =
	{
		FVertexColor(-0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, 0.5f, 1.f, 1.f, 0.f, 1.f)
	};

	unsigned short	CenterCubeColorIdx[36] = { 0, 1, 3, 0, 3, 2,
		1, 5, 7, 1, 7, 3, 5, 4, 6, 5, 6, 7, 4, 0, 2, 4, 2, 6,
		4, 5, 1, 4, 1, 0, 2, 3, 7, 2, 7, 6 };

	if (!CreateMesh("Mesh_CenterCubeColor", CenterCubeColor,
		sizeof(FVertexColor),
		8, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterCubeColorIdx, 2, 36, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;


	// 속이 빈 사각형
	FVector3 CenterFrameRect[4] = {
		FVector3(-0.5f, 0.5f, 0.f),
		FVector3(0.5f, 0.5f, 0.f),
		FVector3(-0.5f, -0.5f, 0.f),
		FVector3(0.5f, -0.5f, 0.f)
	};

	unsigned short CenterFrameRectIdx[5] = { 0,1,3,2,0 };

	if (!CreateMesh("Mesh_CenterFrameRect", CenterFrameRect, sizeof(FVector3),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		CenterFrameRectIdx, 2, 5, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;

	FVector3	LBFrameRect[4] =
	{
		FVector3(0.f, 1.f, 0.f),
		FVector3(1.f, 1.f, 0.f),
		FVector3(0.f, 0.f, 0.f),
		FVector3(1.f, 0.f, 0.f)
	};

	unsigned short	LBFrameRectIdx[5] = { 0, 1, 3, 2, 0 };

	if (!CreateMesh("Mesh_LBFrameRect", LBFrameRect, sizeof(FVector3),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		LBFrameRectIdx, 2, 5, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;

	// FrameSphere2D 생성
	std::vector<FVector3>	FrameSphere2D;

	for (int i = 0; i < 360; i += 12)
	{
		FVector3	Pos;
		Pos.x = cosf(DirectX::XMConvertToRadians((float)i));
		Pos.y = sinf(DirectX::XMConvertToRadians((float)i));

		FrameSphere2D.push_back(Pos);
	}

	std::vector<unsigned short>	FrameSphere2DIdx;

	size_t	PosCount = FrameSphere2D.size();

	for (size_t i = 0; i < PosCount; ++i)
	{
		FrameSphere2DIdx.push_back((unsigned short)i);
	}

	FrameSphere2DIdx.push_back(0);

	if (!CreateMesh("Mesh_FrameSphere2D", &FrameSphere2D[0],
		sizeof(FVector3),
		(int)PosCount, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		&FrameSphere2DIdx[0], 2, (int)FrameSphere2DIdx.size(),
		DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;

	FVertexTex	RectTexture[4] =
	{
		FVertexTex(0.f, 1.f, 0.f, 0.f, 0.f),
		FVertexTex(1.f, 1.f, 0.f, 1.f, 0.f),
		FVertexTex(0.f, 0.f, 0.f, 0.f, 1.f),
		FVertexTex(1.f, 0.f, 0.f, 1.f, 1.f)
	};

	if (!CreateMesh("Mesh_RectTex", RectTexture,
		sizeof(FVertexTex),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterRectColorIdx, 2, 6, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;


	FVector3	LineUp[2] =
	{
		FVector3(0.f, 0.f, 0.f),
		FVector3(0.f, 1.f, 0.f)
	};

	if (!CreateMesh("Mesh_LineUP2D", LineUp,
		sizeof(FVector3),
		2, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP))
		return false;

	FVertexColor LineUpColor[2] =
	{
		FVertexColor(0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f),
		FVertexColor(0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f),
	};

	unsigned short	LineUpIdx[2] = { 0, 1 };

	if (!CreateMesh("Mesh_LineUP2D_Material", LineUpColor,
		sizeof(FVertexColor),
		2, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		LineUpIdx, sizeof(unsigned short), 2, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;

	FVector3	LineRight[2] =
	{
		FVector3(0.f, 0.f, 0.f),
		FVector3(1.f, 0.f, 0.f)
	};

	if (!CreateMesh("Mesh_LineRight2D", LineRight,
		sizeof(FVector3),
		2, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP))
		return false;


	return true;
}

bool CMeshManager::CreateMesh(const std::string& _Name, void* _VertexData, int _VertexSize, int _VertexCount,
	D3D11_USAGE _VertexUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive,
	void* _IndexData, int _IndexSize, int _IndexCount, DXGI_FORMAT _Fmt, D3D11_USAGE _IndexUsage)
{
	std::weak_ptr<CMesh> Check = FindMesh(_Name);

	// 있을 경우
	if (!Check.expired())
		return true;

	std::shared_ptr<CMesh> Mesh;

	Mesh.reset(new CMesh);

	if (!Mesh->CreateMesh(_VertexData, _VertexSize, _VertexCount, _VertexUsage, _Primitive, _IndexData, _IndexSize, _IndexCount,_Fmt, _IndexUsage))
	{
		assert(false);
		return false;
	}

	Mesh->SetName(_Name);

	m_MeshMap.insert(std::make_pair(_Name, Mesh));

	return true;
}

std::weak_ptr<CMesh> CMeshManager::FindMesh(const std::string& _Name)
{
	auto iter = m_MeshMap.find(_Name);

	if (iter == m_MeshMap.end())
		return std::weak_ptr<CMesh>();

	return iter->second;
}

void CMeshManager::ReleaseAsset(const std::string& _Name)
{
	auto iter = m_MeshMap.find(_Name);

	if (iter != m_MeshMap.end())
	{
		// 다른 월드에서 더 이상 들고 있지 않을 경우
		if (iter->second.use_count() == 1)
		{
			m_MeshMap.erase(iter);
		}
	}
}
