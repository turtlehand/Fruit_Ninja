#include "pch.h"
#include "ColliderLine2D.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Shader/ShaderManager.h"
#include "../Asset/Shader/Shader.h"
#include "../Asset/Shader/CBufferCollider.h"
#include "../Asset/Mesh/MeshManager.h"
#include "../Asset/Mesh/Mesh.h"
#include "../World/World.h"
#include "../World/WorldAssetManager.h"
#include "Collision.h"
#include "ColliderBox2D.h"
#include "ColliderSphere2D.h"
#include "ColliderPolygon2D.h"

CColliderLine2D::CColliderLine2D() :
	CCollider(EColliderType::Line2D),
	m_Line2DInfo()
{

}

CColliderLine2D::CColliderLine2D(const CColliderLine2D& _Ref) :
	CCollider(_Ref),
	m_Line2DInfo(_Ref.m_Line2DInfo)
{
}

CColliderLine2D::CColliderLine2D(CColliderLine2D&& _Ref) noexcept :
	CCollider(std::move(_Ref)),
	m_Line2DInfo(std::move(_Ref).m_Line2DInfo)
{
}

CColliderLine2D::~CColliderLine2D()
{
}

void CColliderLine2D::SetDebugDraw(bool DebugDraw)
{
	CCollider::SetDebugDraw(DebugDraw);

	if (DebugDraw && m_Shader.expired())
	{
		std::shared_ptr<CShaderManager>   ShaderMgr =
			CAssetManager::GetInst()->GetShaderManager().lock();

		m_Shader = ShaderMgr->FindShader("Collider");

		auto	World = m_World.lock();

		std::weak_ptr<CMesh>	Mesh;

		if (World)
		{
			auto	AssetMgr = World->GetWorldAssetManager().lock();

			m_Mesh = AssetMgr->FindMesh("LineUP2D");
		}

		else
		{
			std::weak_ptr<CMeshManager> Weak_MeshMgr =
				CAssetManager::GetInst()->GetMeshManager();

			std::shared_ptr<CMeshManager>   MeshMgr = Weak_MeshMgr.lock();

			m_Mesh = MeshMgr->FindMesh("Mesh_LineUP2D");
		}

		m_ColliderCBuffer.reset(new CCBufferCollider);

		m_ColliderCBuffer->Init();
	}
}

bool CColliderLine2D::Init()
{
	CCollider::Init();

	SetDebugDraw(m_DebugDraw);

	return true;
}

void CColliderLine2D::Update(double _DeltaTime)
{
	CCollider::Update(_DeltaTime);
}

void CColliderLine2D::PostUpdate(double _DeltaTime)
{
	CCollider::PostUpdate(_DeltaTime);

	m_Line2DInfo.Start = m_WorldPos;

	// 회전된 선의 방향을 구한다.
	FVector3 Dir;
	Dir = m_LineDir.TransformNormal(m_RotMatrix);
	Dir.Normalize();

	m_Line2DInfo.End = m_Line2DInfo.Start + Dir * m_Distance;


	m_Min.x = m_Line2DInfo.Start.x < m_Line2DInfo.End.x ? m_Line2DInfo.Start.x : m_Line2DInfo.End.x;
	m_Min.y = m_Line2DInfo.Start.y < m_Line2DInfo.End.y ? m_Line2DInfo.Start.y : m_Line2DInfo.End.y;

	m_Max.x = m_Line2DInfo.Start.x > m_Line2DInfo.End.x ? m_Line2DInfo.Start.x : m_Line2DInfo.End.x;
	m_Max.y = m_Line2DInfo.Start.y > m_Line2DInfo.End.y ? m_Line2DInfo.Start.y : m_Line2DInfo.End.y;
	 
	m_RenderScale.x = 1.f;
	m_RenderScale.y = m_Distance;
	m_RenderScale.z = 1.f;
}

CColliderLine2D* CColliderLine2D::Clone()	const
{
	return new CColliderLine2D(*this);
}

bool CColliderLine2D::Collision(std::vector<FVector3>& _HitPoint, std::shared_ptr<CCollider> _Dest)
{
	// 상대방의 충돌체 모양이 무엇이냐에 따라 충돌 알고리즘이 달라진다.
	switch (_Dest->GetColliderType())
	{
	case EColliderType::Box2D:
		return CCollision::CollisionBox2DToLine2D(_HitPoint, dynamic_cast<CColliderBox2D*>(_Dest.get()), this);
		break;
	case EColliderType::Sphere2D:
		return CCollision::CollisionSphere2DToLine2D(_HitPoint, dynamic_cast<CColliderSphere2D*>(_Dest.get()), this);
		break;
	case EColliderType::Polygon2D:
		return CCollision::CollisionPolygon2DToLine2D(_HitPoint, dynamic_cast<CColliderPolygon2D*>(_Dest.get()), this);
		break;
	case EColliderType::Line2D:
		return CCollision::CollisionLine2DToLine2D(_HitPoint, this, dynamic_cast<CColliderLine2D*>(_Dest.get()));
		break;
	}

	return false;
}

