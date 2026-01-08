#include "pch.h"
#include "MeshComponent.h"

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

std::shared_ptr<class CCBufferAnimation2D> CMeshComponent::m_EmptyAnimCBuffer;

CMeshComponent::CMeshComponent()
{
	SetClassType<CMeshComponent>();
	m_RenderType = EComponentRender::Render;
}

CMeshComponent::CMeshComponent(const CMeshComponent& _Ref) : 
	CSceneComponent(_Ref),
	m_Mesh(_Ref.m_Mesh),
	m_Shader(_Ref.m_Shader),
	m_TransformCBuffer(_Ref.m_TransformCBuffer->Clone())
{
	m_RenderType = EComponentRender::Render;
}

CMeshComponent::CMeshComponent(CMeshComponent&& _Ref) noexcept :
	CSceneComponent(_Ref),
	m_Mesh(_Ref.m_Mesh),
	m_Shader(_Ref.m_Shader),
	m_TransformCBuffer(_Ref.m_TransformCBuffer->Clone())
{
	m_RenderType = EComponentRender::Render;

	_Ref.m_Mesh.reset();
	_Ref.m_Shader.reset();
	_Ref.m_TransformCBuffer.reset();
}

CMeshComponent::~CMeshComponent()
{
}

void CMeshComponent::SetMesh(const std::weak_ptr<CMesh>& _Mesh)
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

void CMeshComponent::SetMesh(const std::string& _Name)
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

void CMeshComponent::SetShader(const std::weak_ptr<CShader>& _Shader)
{
	m_Shader = _Shader;
}

void CMeshComponent::SetShader(const std::string& _Name)
{
	auto World = m_World.lock();

	std::weak_ptr<CShaderManager> Weak_ShaderMgr = CAssetManager::GetInst()->GetShaderManager();

	std::shared_ptr<CShaderManager> ShaderMgr = Weak_ShaderMgr.lock();

	SetShader(ShaderMgr->FindShader(_Name));
}

bool CMeshComponent::Init()
{
	CSceneComponent::Init();
	m_TransformCBuffer.reset(new CCBufferTransform);
	m_TransformCBuffer->Init();
	return true;
}

void CMeshComponent::Render()
{
	// 셰이더 또는 메쉬가 없다면 그리지 않는다.
	if (m_Shader.expired() || m_Mesh.expired())
		return;

	std::shared_ptr<CShader> Shader = m_Shader.lock();
	std::shared_ptr<CMesh> Mesh = m_Mesh.lock();

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

		if(m_MaterialSlot[i])
			m_MaterialSlot[i]->Reset();
	}

	CSceneComponent::Render();

}

CMeshComponent* CMeshComponent::Clone() const
{
	return new CMeshComponent(*this);
}

void CMeshComponent::Destroy()
{
	CSceneComponent::Destroy();
}

void CMeshComponent::SetMaterialBaseColor(int _SlotIndex, float _r, float _g, float _b, float _a)
{
	m_MaterialSlot[_SlotIndex]->SetBaseColor(_r, _g, _b, _a);
}

void CMeshComponent::SetMaterialBaseColor(int _SlotIndex, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	m_MaterialSlot[_SlotIndex]->SetBaseColor(_r, _g, _b, _a);
}

void CMeshComponent::SetMaterialBaseColor(int _SlotIndex, const FVector4& _Color)
{
	m_MaterialSlot[_SlotIndex]->SetBaseColor(_Color);

}

void CMeshComponent::SetMaterialOpacity(int _SlotIndex, float _Opacity)
{
	m_MaterialSlot[_SlotIndex]->SetOpacity(_Opacity);
}

/// <summary>
/// _SlotIndex번째 재질의 _TextureIndex번째 텍스쳐를 _Texture로 변경시켜준다.
/// </summary>
/// <param name="_SlotIndex"></param>
/// <param name="_TextureIndex"></param>
/// <param name="_Texture"></param>
bool CMeshComponent::SetTexture(int _SlotIndex, int _TextureIndex,
	const std::weak_ptr<CTexture> _Texture)
{
	if ((int)m_MaterialSlot.size() <= _SlotIndex)
		return false;

	return m_MaterialSlot[_SlotIndex]->SetTexture(_TextureIndex, _Texture);
}

void CMeshComponent::AddTexture(int _SlotIndex,
	const std::weak_ptr<CTexture>& _Texture,
	int _Register, int _ShaderBufferType,
	int _Index )
{
	m_MaterialSlot[_SlotIndex]->AddTexture(_Texture, _Register, _ShaderBufferType, _Index);
}

void CMeshComponent::AddTexture(int _SlotIndex,
	const std::string& _Name,
	int _Register, int _ShaderBufferType ,
	int _Index)
{
	auto	World = m_World.lock();

	std::weak_ptr<CTexture>	Texture;

	auto	AssetMgr = World->GetWorldAssetManager().lock();

	Texture = AssetMgr->FindTexture(_Name);

	m_MaterialSlot[_SlotIndex]->AddTexture(Texture, _Register,
		_ShaderBufferType, _Index);
}

void CMeshComponent::AddTexture(int _SlotIndex,
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

void CMeshComponent::AddTextureFullPath(int _SlotIndex,
	const std::string& _Name,
	const wchar_t* _FullPath,
	int _Register, int _ShaderBufferType,
	int _Index )
{
	auto	World = m_World.lock();

	auto	AssetMgr = World->GetWorldAssetManager().lock();

	AssetMgr->LoadTextureFullPath(_Name, _FullPath);

	std::weak_ptr<CTexture>	Texture = AssetMgr->FindTexture(_Name);

	m_MaterialSlot[_SlotIndex]->AddTexture(Texture,
		_Register, _ShaderBufferType, _Index);
}

void CMeshComponent::AddTexture(int _SlotIndex,
	const std::string& _Name,
	const std::vector<const wchar_t*>& _FileName,
	const std::wstring& _PathName ,
	int _Register, int _ShaderBufferType,
	int _Index )
{
	auto	World = m_World.lock();

	auto	AssetMgr = World->GetWorldAssetManager().lock();

	AssetMgr->LoadTexture(_Name, _FileName, _PathName);

	std::weak_ptr<CTexture>	Texture = AssetMgr->FindTexture(_Name);

	m_MaterialSlot[_SlotIndex]->AddTexture(Texture,
		_Register, _ShaderBufferType, _Index);
}

void CMeshComponent::AddTextureFullPath(int _SlotIndex,
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
void CMeshComponent::SetBlendState(int _SlotIndex, const std::string& _Name)
{
	m_MaterialSlot[_SlotIndex]->SetBlendState(_Name);
}

/// <summary>
/// 0번 재질에 _Name인 BlendState를 설정한다.
/// </summary>
/// <param name="_SlotIndex"></param>
/// <param name="_State"></param>
void CMeshComponent::SetBlendState(int _SlotIndex, const std::weak_ptr<class CRenderState>& _State)
{
	m_MaterialSlot[_SlotIndex]->SetBlendState(_State);
}

void CMeshComponent::CreateEmptyAnimCBuffer()
{
	m_EmptyAnimCBuffer.reset(new CCBufferAnimation2D);

	m_EmptyAnimCBuffer->Init();

	m_EmptyAnimCBuffer->SetAnimation2DEnable(false);
}

void CMeshComponent::ClearEmptyAnimCBuffer()
{
	m_EmptyAnimCBuffer.reset();
}
