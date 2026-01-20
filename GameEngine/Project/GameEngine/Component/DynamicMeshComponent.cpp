#include "pch.h"
#include "DynamicMeshComponent.h"

#include "Asset/AssetManager.h"
#include "Asset/Mesh/MeshManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Asset/Shader/GraphicShader.h"
#include "Asset/Shader/ShaderManager.h"
#include "Asset/Material/Material.h"
#include "../Asset/Shader/CBufferTransform.h"

#include "CameraComponent.h"
#include "../World/World.h"
#include "../World/CameraManager.h"

#include "Render/RenderManager.h"

#include "Animation2DComponent.h"
#include "../Asset/Shader/CBufferAnimation2D.h"

#include "Component/Collision.h"

std::shared_ptr<class CCBufferAnimation2D> CDynamicMeshComponent::m_EmptyAnimCBuffer;

CDynamicMeshComponent::CDynamicMeshComponent()
{
	SetClassType<CDynamicMeshComponent>();
	m_RenderType = EComponentRender::Render;
}

CDynamicMeshComponent::CDynamicMeshComponent(const CDynamicMeshComponent& _Ref) :
	CSceneComponent(_Ref),
	m_Shader(_Ref.m_Shader),
	m_VertexPath(_Ref.m_VertexPath),
	m_TransformCBuffer(_Ref.m_TransformCBuffer->Clone())
{
	CreateDynamicMesh();

	m_RenderType = EComponentRender::Render;
}

CDynamicMeshComponent::CDynamicMeshComponent(CDynamicMeshComponent&& _Ref) noexcept :
	CSceneComponent(_Ref),
	m_Shader(_Ref.m_Shader),
	m_VertexPath(std::move(_Ref.m_VertexPath)),
	m_TransformCBuffer(_Ref.m_TransformCBuffer->Clone())
{
	//m_DynamicMesh = std::move(_Ref.m_DynamicMesh);

	m_RenderType = EComponentRender::Render;

	_Ref.m_DynamicMesh.reset();
	_Ref.m_Shader.reset();
	_Ref.m_TransformCBuffer.reset();
}

CDynamicMeshComponent::~CDynamicMeshComponent()
{
}

int CDynamicMeshComponent::GetAllVertexCount() const
{
	int Size = 0;
	for (int i = 0; i < m_PathSize; ++i)
	{
		Size += m_VertexPath[i].size();
	}
	return Size;
}

void CDynamicMeshComponent::ResizePath(int _PathSize)
{
	m_PathSize = _PathSize;
	m_VertexPath.resize(_PathSize);
}

/*
void CDynamicMeshComponent::SetMesh(const std::weak_ptr<CMesh>& _Mesh)
{
	m_Mesh = _Mesh;

	std::shared_ptr<CMesh> Mesh = m_Mesh.lock();

	// Mesh의 FMeshSlot을 복사하여 가지고 있는다.
	if (Mesh)
	{
		m_MaterialSlot.clear();

		size_t SlotCount = Mesh->GetMeshSlotCount();

		for (size_t i = 0; i < SlotCount; ++i)
		{
			std::shared_ptr<FMeshSlot> Slot = Mesh->GetMeshSlot((int)i);

			CMaterial* Material = Slot->Material->Clone();

			std::shared_ptr<CMaterial> Mtrl(Material);

			m_MaterialSlot.push_back(Mtrl);
		}
	}
}

void CDynamicMeshComponent::SetMesh(const std::string& _Name)
{
	auto World = m_World.lock();

	std::weak_ptr<CMesh> Mesh;

	// 현재 World가 있다면 WorldAssetManager에서 가져온다.
	if (World)
	{
		auto AssetMgr = World->GetWorldAssetManager().lock();

		Mesh = AssetMgr->FindMesh(_Name);

		if (Mesh.expired())
			return;

		SetMesh(Mesh);
	}
	// 현재 World가 없다면 AssetManager에서 가져온다.
	else
	{
		std::weak_ptr<CMeshManager> Weak_MeshMgr = CAssetManager::GetInst()->GetMeshManager();

		std::shared_ptr<CMeshManager> MeshMgr = Weak_MeshMgr.lock();

		SetMesh(MeshMgr->FindMesh(_Name));
	}

}
*/

void CDynamicMeshComponent::AddVertex(const FVector3& _Point, int _Path)
{
	// 더 이상 추가 불가능
	if (m_MaxPoint <= (GetAllVertexCount() + 1))
	{
		assert(false);
		return;
	}
	// 잘못된 범위
	else if (_Path < 0 || m_PathSize <= _Path)
		return;

	m_VertexPath[_Path].push_back(_Point);

	// 정점, 인덱스 버퍼 변경하기
	// 속이 빈 다각형
	UpdateMesh();

}

/// <summary>
/// _Index의 정점을 _Point로 수정한다.
/// </summary>
/// <param name="_Point"></param>
void CDynamicMeshComponent::SetVertex(int _Index, const FVector3& _Point, int _Path)
{
	// 잘못된 범위
	if (!(0 <= _Index && _Index < m_MaxPoint))
	{
		assert(false);
		return;
	}
	// 잘못된 범위
	else if (_Path < 0 || m_PathSize <= _Path)
		return;

	// Info를 변경한다.
	m_VertexPath[_Path][_Index] = _Point;

	// 정점, 인덱스 버퍼 변경하기
	// 속이 빈 다각형
	UpdateMesh();
}

/// <summary>
/// 마지막 점을 삭제한다.
/// </summary>
void CDynamicMeshComponent::RemoveVertex(int _Path)
{
	// 잘못된 범위
	if (m_VertexPath.empty())
	{
		assert(false);
		return;
	}

	m_VertexPath[_Path].pop_back();

	// 정점, 인덱스 버퍼 변경하기
	// 속이 빈 다각형
	UpdateMesh();
}


void CDynamicMeshComponent::SetShader(const std::weak_ptr<CShader>& _Shader)
{
	m_Shader = _Shader;
}

void CDynamicMeshComponent::SetShader(const std::string& _Name)
{
	auto World = m_World.lock();

	std::weak_ptr<CShaderManager> Weak_ShaderMgr = CAssetManager::GetInst()->GetShaderManager();

	std::shared_ptr<CShaderManager> ShaderMgr = Weak_ShaderMgr.lock();

	SetShader(ShaderMgr->FindShader(_Name));
}

bool CDynamicMeshComponent::Init()
{
	CSceneComponent::Init();
	CreateDynamicMesh();
	UpdateMesh();
	ResizePath(1);
	m_TransformCBuffer.reset(new CCBufferTransform);
	m_TransformCBuffer->Init();
	return true;
}

void CDynamicMeshComponent::Render()
{
	// 셰이더 또는 메쉬가 없다면 그리지 않는다.
	if (m_Shader.expired() || !m_DynamicMesh)
		return;

	std::shared_ptr<CShader> Shader = m_Shader.lock();
	std::shared_ptr<CMesh> Mesh = m_DynamicMesh;

	FMatrix ViewMat;
	FMatrix ProjMat;

	auto World = m_World.lock();

	if (World)
	{
		auto CameraMgr = World->GetCameraManager().lock();

		if (CameraMgr)
		{
			ViewMat = CameraMgr->GetViewMatrix();
			ProjMat = CameraMgr->GetProjMatrix();
		}
	}

	// TransformBuffer에게 보내주고
	m_TransformCBuffer->SetWorldMatrix(m_WorldMatrix);
	m_TransformCBuffer->SetViewMatrix(ViewMat);
	m_TransformCBuffer->SetProjMatrix(ProjMat);

	FVector3 PivotSize = m_Pivot * Mesh->GetMeshSize();

	m_TransformCBuffer->SetPivotSize(PivotSize);

	// 데이터를 바인딩한다.
	m_TransformCBuffer->UpdateBuffer();

	// Shader 정보를 바인딩한다.
	Shader->SetShader();


	auto Anim = m_AnimComponent.lock();

	if (Anim)
		Anim->SetShader();
	else
		m_EmptyAnimCBuffer->UpdateBuffer();


	// 슬롯 수만큼 반복하며 각각의 슬롯을 출력한다.
	size_t Size = m_MaterialSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!m_AnimComponent.expired())
		{
			// Sprite Texture를 사용할 경우
			if (Anim->GetTextureType() == EAnimation2DTextureType::SpriteSheet)
			{
				if (m_MaterialSlot[i])
					m_MaterialSlot[i]->UpdateConstantBuffer();
			}

			// 낱장단위 Texture를 사용할 경우
			else if (Anim->GetTextureType() == EAnimation2DTextureType::Frame)
			{
				if (m_MaterialSlot[i])
					m_MaterialSlot[i]->UpdateConstantBuffer(Anim->GetAnimationFrame());
			}
		}
		else
		{
			if (m_MaterialSlot[i])
				m_MaterialSlot[i]->UpdateConstantBuffer();
		}



		Mesh->Render((int)i);

		if (m_MaterialSlot[i])
			m_MaterialSlot[i]->Reset();
	}

	CSceneComponent::Render();

}

CDynamicMeshComponent* CDynamicMeshComponent::Clone() const
{
	return new CDynamicMeshComponent(*this);
}

void CDynamicMeshComponent::Destroy()
{
	CSceneComponent::Destroy();
}

void CDynamicMeshComponent::SetMaterialBaseColor(int _SlotIndex, float _r, float _g, float _b, float _a)
{
	m_MaterialSlot[_SlotIndex]->SetBaseColor(_r, _g, _b, _a);
}

void CDynamicMeshComponent::SetMaterialBaseColor(int _SlotIndex, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	m_MaterialSlot[_SlotIndex]->SetBaseColor(_r, _g, _b, _a);
}

void CDynamicMeshComponent::SetMaterialBaseColor(int _SlotIndex, const FVector4& _Color)
{
	m_MaterialSlot[_SlotIndex]->SetBaseColor(_Color);

}

void CDynamicMeshComponent::SetMaterialOpacity(int _SlotIndex, float _Opacity)
{
	m_MaterialSlot[_SlotIndex]->SetOpacity(_Opacity);
}

/// <summary>
/// _SlotIndex번째 재질의 _TextureIndex번째 텍스쳐를 _Texture로 변경시켜준다.
/// </summary>
/// <param name="_SlotIndex"></param>
/// <param name="_TextureIndex"></param>
/// <param name="_Texture"></param>
bool CDynamicMeshComponent::SetTexture(int _SlotIndex, int _TextureIndex,
	const std::weak_ptr<CTexture> _Texture)
{
	if ((int)m_MaterialSlot.size() <= _SlotIndex)
		return false;

	return m_MaterialSlot[_SlotIndex]->SetTexture(_TextureIndex, _Texture);
}

const std::weak_ptr<CTexture>& CDynamicMeshComponent::GetTexture(int _SlotIndex, int _TextureIndex)
{
	if ((int)m_MaterialSlot.size() <= _SlotIndex)
		return std::weak_ptr<CTexture>();

	return m_MaterialSlot[_SlotIndex]->GetTexture(_TextureIndex);
}

void CDynamicMeshComponent::AddTexture(int _SlotIndex,
	const std::weak_ptr<CTexture>& _Texture,
	int _Register, int _ShaderBufferType,
	int _Index)
{
	m_MaterialSlot[_SlotIndex]->AddTexture(_Texture, _Register, _ShaderBufferType, _Index);
}

void CDynamicMeshComponent::AddTexture(int _SlotIndex,
	const std::string& _Name,
	int _Register, int _ShaderBufferType,
	int _Index)
{
	auto	World = m_World.lock();

	std::weak_ptr<CTexture>	Texture;

	auto	AssetMgr = World->GetWorldAssetManager().lock();

	Texture = AssetMgr->FindTexture(_Name);

	m_MaterialSlot[_SlotIndex]->AddTexture(Texture, _Register,
		_ShaderBufferType, _Index);
}

void CDynamicMeshComponent::AddTexture(int _SlotIndex,
	const std::string& _Name,
	const wchar_t* _FileName,
	const std::wstring& _PathName,
	int _Register, int _ShaderBufferType,
	int _Index)
{
	auto	World = m_World.lock();

	auto	AssetMgr = World->GetWorldAssetManager().lock();

	AssetMgr->LoadTexture(_Name, _FileName, _PathName);

	std::weak_ptr<CTexture>	Texture = AssetMgr->FindTexture(_Name);

	m_MaterialSlot[_SlotIndex]->AddTexture(Texture, _Register, _ShaderBufferType, _Index);
}

void CDynamicMeshComponent::AddTextureFullPath(int _SlotIndex,
	const std::string& _Name,
	const wchar_t* _FullPath,
	int _Register, int _ShaderBufferType,
	int _Index)
{
	auto	World = m_World.lock();

	auto	AssetMgr = World->GetWorldAssetManager().lock();

	AssetMgr->LoadTextureFullPath(_Name, _FullPath);

	std::weak_ptr<CTexture>	Texture = AssetMgr->FindTexture(_Name);

	m_MaterialSlot[_SlotIndex]->AddTexture(Texture,
		_Register, _ShaderBufferType, _Index);
}

void CDynamicMeshComponent::AddTexture(int _SlotIndex,
	const std::string& _Name,
	const std::vector<const wchar_t*>& _FileName,
	const std::wstring& _PathName,
	int _Register, int _ShaderBufferType,
	int _Index)
{
	auto	World = m_World.lock();

	auto	AssetMgr = World->GetWorldAssetManager().lock();

	AssetMgr->LoadTexture(_Name, _FileName, _PathName);

	std::weak_ptr<CTexture>	Texture = AssetMgr->FindTexture(_Name);

	m_MaterialSlot[_SlotIndex]->AddTexture(Texture,
		_Register, _ShaderBufferType, _Index);
}

void CDynamicMeshComponent::AddTextureFullPath(int _SlotIndex,
	const std::string& _Name,
	const std::vector<const wchar_t*>& _FullPath,
	int _Register, int _ShaderBufferType,
	int _Index)
{
	auto	World = m_World.lock();

	std::weak_ptr<CTexture>	Texture;

	auto	AssetMgr = World->GetWorldAssetManager().lock();

	AssetMgr->LoadTextureFullPath(_Name, _FullPath);

	Texture = AssetMgr->FindTexture(_Name);

	m_MaterialSlot[_SlotIndex]->AddTexture(Texture,
		_Register, _ShaderBufferType, _Index);
}

/// <summary>
/// 0번 재질에 _Name인 BlendState를 설정한다.
/// </summary>
/// <param name="_SlotIndex"></param>
/// <param name="_Name"></param>
void CDynamicMeshComponent::SetBlendState(int _SlotIndex, const std::string& _Name)
{
	m_MaterialSlot[_SlotIndex]->SetBlendState(_Name);
}

/// <summary>
/// 0번 재질에 _Name인 BlendState를 설정한다.
/// </summary>
/// <param name="_SlotIndex"></param>
/// <param name="_State"></param>
void CDynamicMeshComponent::SetBlendState(int _SlotIndex, const std::weak_ptr<class CRenderState>& _State)
{
	m_MaterialSlot[_SlotIndex]->SetBlendState(_State);
}

void CDynamicMeshComponent::CreateEmptyAnimCBuffer()
{
	m_EmptyAnimCBuffer.reset(new CCBufferAnimation2D);

	m_EmptyAnimCBuffer->Init();

	m_EmptyAnimCBuffer->SetAnimation2DEnable(false);
}

void CDynamicMeshComponent::ClearEmptyAnimCBuffer()
{
	m_EmptyAnimCBuffer.reset();
}

std::weak_ptr<class CMesh> CDynamicMeshComponent::CreateDynamicMesh()
{
	// 속이 빈 사각형
	std::vector<FVertexTex> CenterFrame(m_MaxPoint, FVertexTex());
	std::vector<unsigned short> CenterFrameIdx((m_MaxPoint - 2) * 3, 0);

	std::shared_ptr<CMesh> Mesh;

	Mesh.reset(new CMesh);

	if (!Mesh->CreateMesh(CenterFrame.data(), sizeof(FVertexTex), m_MaxPoint, D3D11_USAGE_DYNAMIC, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterFrameIdx.data(), sizeof(unsigned short), (m_MaxPoint - 2) * 3, DXGI_FORMAT_R16_UINT, D3D11_USAGE_DYNAMIC))
	{
		assert(false);
		return std::weak_ptr<CMesh>();
	}

	m_DynamicMesh = Mesh;

	// Mesh의 FMeshSlot을 복사하여 가지고 있는다.
	if (Mesh)
	{
		m_MaterialSlot.clear();

		size_t SlotCount = Mesh->GetMeshSlotCount();

		for (size_t i = 0; i < SlotCount; ++i)
		{
			std::shared_ptr<FMeshSlot> Slot = Mesh->GetMeshSlot((int)i);

			CMaterial* Material = Slot->Material->Clone();

			std::shared_ptr<CMaterial> Mtrl(Material);

			m_MaterialSlot.push_back(Mtrl);
		}
	}

	return Mesh;
}

/// <summary>
/// 점을 이용하여 삼각형으로 분할하고 메쉬를 재설정한다.
/// </summary>
/// <param name="_Triangle"></param>
void CDynamicMeshComponent::UpdateMesh()
{
	// 더 이상 추가 불가능
	if (m_MaxPoint <= GetAllVertexCount())
	{
		assert(false);
		return;
	}

	// 정점, 인덱스 버퍼 변경하기
	// 속이 빈 다각형
	std::vector<FVertexTex> CenterFrame;
	CenterFrame.reserve(m_MaxPoint);
	std::vector<unsigned short> CenterFrameIdx;
	CenterFrameIdx.reserve((m_MaxPoint - 2) * 3);

	for (int Path = 0; Path < m_PathSize; ++Path)
	{
		// 삼각형 분할
		//m_Polygon2DInfo.LocalTriangle.clear();
		std::vector<FTriangle2DInfo> Triangle;
		CCollision::EarClipping(m_VertexPath[Path], Triangle);

		for (int i = 0; i < Triangle.size(); ++i)
		{

			for (int j = 0; j < 3; ++j)
			{
				// 1. 이미 정점 리스트에 있는 점인지 확인 (중복 제거)
				int FoundIndex = -1;
				for (int v = 0; v < (int)CenterFrame.size(); ++v)
				{
					if (CenterFrame[v].Pos == Triangle[i].Point[j]) // 근사치 비교 권장
					{
						FoundIndex = v;
						break;
					}
				}

				// 2. 새로운 점이면 Vertices에 추가
				if (FoundIndex == -1)
				{
					FoundIndex = (int)CenterFrame.size();
					CenterFrame.push_back(FVertexTex(Triangle[i].Point[j], GetUVFromPos(Triangle[i].Point[j])));
				}

				// 3. 인덱스 버퍼에 해당 번호 기록
				CenterFrameIdx.push_back((unsigned short)FoundIndex);

			}
		}
	}

	CenterFrame.resize(m_MaxPoint);
	CenterFrameIdx.resize((m_MaxPoint - 2) * 3);

	m_DynamicMesh->ChangeVertexBuffer(CenterFrame.data(), sizeof(FVertexTex), CenterFrame.size());
	m_DynamicMesh->ChangeIndexBuffer(0, CenterFrameIdx.data(), sizeof(unsigned short), CenterFrameIdx.size());
}
/// <summary>
/// 입력된 삼각형을 이용하여 메쉬의 정점, 인덱스를 변경한다.
/// </summary>
/// <param name="_Triangle"></param>
void CDynamicMeshComponent::UpdateMesh(const std::vector<FTriangle2DInfo>& _Triangle)
{

	// 정점, 인덱스 버퍼 변경하기
	// 속이 빈 다각형
	std::vector<FVector3> CenterFrame;
	std::vector<unsigned short> CenterFrameIdx;

	for (int i = 0; i < _Triangle.size(); ++i)
	{

		for (int j = 0; j < 3; ++j)
		{
			// 1. 이미 정점 리스트에 있는 점인지 확인 (중복 제거)
			int FoundIndex = -1;
			for (int v = 0; v < (int)CenterFrame.size(); ++v)
			{
				if (CenterFrame[v] == _Triangle[i].Point[j]) // 근사치 비교 권장
				{
					FoundIndex = v;
					break;
				}
			}

			// 2. 새로운 점이면 Vertices에 추가
			if (FoundIndex == -1)
			{
				FoundIndex = (int)CenterFrame.size();
				CenterFrame.push_back(_Triangle[i].Point[j]);
			}

			// 3. 인덱스 버퍼에 해당 번호 기록
			CenterFrameIdx.push_back((unsigned short)FoundIndex);

		}
	}

	CenterFrame.resize(m_MaxPoint);
	CenterFrameIdx.resize((m_MaxPoint - 2) * 3);

	m_DynamicMesh->ChangeVertexBuffer(CenterFrame.data(), sizeof(FVector3), CenterFrame.size());
	m_DynamicMesh->ChangeIndexBuffer(0, CenterFrameIdx.data(), sizeof(unsigned short), CenterFrameIdx.size());
}