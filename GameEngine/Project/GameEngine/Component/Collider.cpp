#include "pch.h"
#include "Collider.h"

#include "../Render/RenderManager.h"
#include "../Asset/Shader/CBufferTransform.h"
#include "../Asset/Shader/CBufferCollider.h"
#include "../World/CameraManager.h"
#include "../World/World.h"
#include "../World/WorldCollision.h"
#include "../CollisionInfoManager.h"

#include "../Asset/Shader/Shader.h"
#include "../Asset/Mesh/Mesh.h"


CCollider::CCollider(EColliderType _ColliderType) :
	CSceneComponent(),
	m_ColliderType(_ColliderType),
	m_Min(),
	m_Max(),
	m_RenderScale(),
	m_DebugDraw(false),
	m_Collision(false)
{
}

CCollider::CCollider(const CCollider& _Ref) :
	CSceneComponent(_Ref),
	m_ColliderType(_Ref.m_ColliderType),
	m_Min(_Ref.m_Min),
	m_Max(_Ref.m_Max),
	m_RenderScale(_Ref.m_RenderScale),
	m_DebugDraw(_Ref.m_DebugDraw),
	m_Collision(_Ref.m_Collision)
{
}

CCollider::CCollider(CCollider&& _Ref) noexcept :
	CSceneComponent(std::move(_Ref)),
	m_ColliderType(std::move(_Ref.m_ColliderType)),
	m_Min(std::move(_Ref.m_Min)),
	m_Max(std::move(_Ref.m_Max)),
	m_RenderScale(std::move(_Ref.m_RenderScale)),
	m_DebugDraw(std::move(_Ref.m_DebugDraw)),
	m_Collision(std::move(_Ref.m_Collision))
{
}

CCollider::~CCollider()
{
	// 파괴될 때 자신과 충돌 중인 대상에서 자신을 삭제해준다.
	auto	iter = m_CollisionObjectMap.begin();
	auto	iterEnd = m_CollisionObjectMap.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->second.expired())
			continue;

		auto	Dest = iter->second.lock();

		Dest->EraseCollisionObject(this);
		//CallCollisionEnd(Dest.get());
		//Dest->CallCollisionEnd(this);
	}
}

void CCollider::SetDebugDraw(bool _Draw)
{
	m_DebugDraw = _Draw;

	if (m_DebugDraw)
	{
		m_RenderType = EComponentRender::Render;

		auto self = std::dynamic_pointer_cast<CSceneComponent>(m_Self.lock());

		CRenderManager::GetInst()->AddRenderLayer(self);

		m_TransformCBuffer.reset(new CCBufferTransform);

		m_TransformCBuffer->Init();
	}
}

void CCollider::SetCollisionProfile(const std::string& _Name)
{
	m_Profile = CCollisionInfoManager::GetInst()->FindProfile(_Name);
}

bool CCollider::Init()
{
	CSceneComponent::Init();

	SetDebugDraw(m_DebugDraw);

	m_Profile = CCollisionInfoManager::GetInst()->FindProfile("Static");

	// WorldCollision에 자신을 등록한다.
	auto	World = m_World.lock();

	if (World)
	{
		auto	CollisionMgr = World->GetCollision().lock();

		if (CollisionMgr)
		{
			CollisionMgr->AddCollider(std::dynamic_pointer_cast<CCollider>(m_Self.lock()));
		}
	}

	return true;
}

void CCollider::Update(double _DeltaTime)
{
	CSceneComponent::Update(_DeltaTime);
}

void CCollider::PostUpdate(double _DeltaTime)
{
	CSceneComponent::PostUpdate(_DeltaTime);
}

void CCollider::Render()
{
	CSceneComponent::Render();

	if (!m_DebugDraw)
		return;

	// 콜라이더 정보를 바탕으로 World 행렬을 만들고 Transform을 바인딩한다.

	// View, Proj 행렬을 가져온다.
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

	auto Mesh = m_Mesh.lock();

	FMatrix ScaleMatrix, RotMatrix, TranslateMatrix, WorldMatrix;

	ScaleMatrix.Scaling(m_RenderScale);
	RotMatrix.Rotation(m_WorldRot);
	TranslateMatrix.Translation(m_WorldPos);

	WorldMatrix = ScaleMatrix * RotMatrix * TranslateMatrix;

	// TransformBuffer에게 보내주고
	m_TransformCBuffer->SetWorldMatrix(WorldMatrix);
	m_TransformCBuffer->SetViewMatrix(ViewMat);
	m_TransformCBuffer->SetProjMatrix(ProjMat);

	FVector3 PivotSize = m_Pivot * Mesh->GetMeshSize();

	m_TransformCBuffer->SetPivotSize(PivotSize);

	// 데이터를 바인딩한다.
	m_TransformCBuffer->UpdateBuffer();

	// 콜라이더 상태를 바인딩한다.
	if (m_Collision)
		m_ColliderCBuffer->SetColor(FVector4::Red);
	else
		m_ColliderCBuffer->SetColor(FVector4::Green);

	m_ColliderCBuffer->UpdateBuffer();

	auto Shader = m_Shader.lock();
	Shader->SetShader();

	Mesh->Render();

}

bool CCollider::CheckCollisionObject(CCollider* _Collider)
{
	auto iter = m_CollisionObjectMap.find(_Collider);

	if (iter == m_CollisionObjectMap.end())
		return false;

	return true;
}

/// <summary>
/// 콜라이더 등록을 해제한다.
/// </summary>
/// <param name="_Collider"></param>
void CCollider::EraseCollisionObject(CCollider* _Collider)
{
	auto iter = m_CollisionObjectMap.find(_Collider);

	if (iter == m_CollisionObjectMap.end())
		return;

	m_CollisionObjectMap.erase(_Collider);

	// 콜라이더가 완전히 없어진다면 충돌 상태를 해제한다.
	if (m_CollisionObjectMap.empty())
		m_Collision = false;
}


void CCollider::CallCollisionBegin(const std::vector<FVector3>& _HitPoint, std::weak_ptr<CCollider>& _Collider)
{
	auto Collider = _Collider.lock();

	m_CollisionObjectMap.insert(std::make_pair(Collider.get(), _Collider));

	m_Collision = true;

	if (m_CollisionBeginFunc)
		m_CollisionBeginFunc(_HitPoint, Collider.get());
}

void CCollider::CallCollision(const std::vector<FVector3>& _HitPoint, std::weak_ptr<CCollider>& _Collider)
{
	auto Collider = _Collider.lock();

	m_CollisionObjectMap.insert(std::make_pair(Collider.get(), _Collider));

	m_Collision = true;

	if (m_CollisionFunc)
		m_CollisionFunc(_HitPoint, Collider.get());
}

void CCollider::CallCollisionEnd(std::weak_ptr<CCollider>& _Collider)
{
	auto Collider = _Collider.lock();

	m_CollisionObjectMap.erase(Collider.get());

	if (m_CollisionObjectMap.empty())
		m_Collision = false;

	if (m_CollisionEndFunc)
		m_CollisionEndFunc(Collider.get());
}
