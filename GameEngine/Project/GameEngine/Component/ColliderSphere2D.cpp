#include "pch.h"
#include "ColliderSphere2D.h"


#include "../Asset/AssetManager.h"
#include "../Asset/Shader/ShaderManager.h"

#include "../Render/RenderManager.h"
#include "../Asset/Shader/CBufferTransform.h"
#include "../Asset/Shader/CBufferCollider.h"
#include "../World/CameraManager.h"
#include "../World/World.h"

#include "../Asset/Shader/Shader.h"
#include "../Asset/Mesh/Mesh.h"

#include "Collision.h"
#include "Collider.h"
#include "ColliderBox2D.h"
#include "ColliderLine2D.h"

CColliderSphere2D::CColliderSphere2D() :
    CCollider(EColliderType::Sphere2D),
    m_Sphere2DInfo()
{

}

CColliderSphere2D::CColliderSphere2D(const CColliderSphere2D& _Ref) :
    CCollider(_Ref),
    m_Sphere2DInfo(_Ref.m_Sphere2DInfo)
{
}

CColliderSphere2D::CColliderSphere2D(CColliderSphere2D&& _Ref) noexcept :
    CCollider(std::move(_Ref)),
    m_Sphere2DInfo(std::move(_Ref.m_Sphere2DInfo))
{
}

CColliderSphere2D::~CColliderSphere2D()
{
}

void CColliderSphere2D::SetDebugDraw(bool _Draw)
{
    CCollider::SetDebugDraw(_Draw);

    if (_Draw && m_Shader.expired())
    {
        std::shared_ptr<CShaderManager> ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock();

        m_Shader = ShaderMgr->FindShader("Collider");

        auto World = m_World.lock();

        if (World)
        {
            auto  AssetMgr = World->GetWorldAssetManager().lock();

            m_Mesh = AssetMgr->FindMesh("FrameSphere2D");
        }
        else
        {
            std::shared_ptr<CMeshManager> MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();

            m_Mesh = MeshMgr->FindMesh("Mesh_FrameSphere2D");
        }

        m_ColliderCBuffer.reset(new CCBufferCollider);

        m_ColliderCBuffer->Init();
    }
}

bool CColliderSphere2D::Init()
{
    CCollider::Init();
    SetDebugDraw(m_DebugDraw);
    return true;
}

void CColliderSphere2D::Update(double _DeltaTime)
{
    CCollider::Update(_DeltaTime);
}

void CColliderSphere2D::PostUpdate(double _DeltaTime)
{
    CCollider::PostUpdate(_DeltaTime);

    m_Sphere2DInfo.Center = m_WorldPos;

    m_RenderScale.x = m_WorldScale.x * m_Sphere2DInfo.Radius;
    m_RenderScale.y = m_WorldScale.y * m_Sphere2DInfo.Radius;
    m_RenderScale.z = 1.f;
}

CColliderSphere2D* CColliderSphere2D::Clone() const
{
    return new CColliderSphere2D(*this);
}


bool CColliderSphere2D::Collision(FVector3& _HitPoint, std::shared_ptr<CCollider> _Dest)
{
    // 상대방의 충돌체 모양이 무엇이냐에 따라 충돌 알고리즘이 달라진다.
    switch (_Dest->GetColliderType())
    {
    case EColliderType::Box2D:
        // 둘다 회전이 0일 경우 AABB, 아니면 OBB 충돌을 진행한다.
        return CCollision::CollisionBox2DToSphere2D(_HitPoint, dynamic_cast<CColliderBox2D*>(_Dest.get()), this);
        break;
    case EColliderType::Sphere2D:
        return CCollision::CollisionSphere2DToSphere2D(_HitPoint, dynamic_cast<CColliderSphere2D*>(_Dest.get()), this);
        break;
    case EColliderType::Line2D:
        return CCollision::CollisionSphere2DToLine2D(_HitPoint, this, dynamic_cast<CColliderLine2D*>(_Dest.get()));
        break;
    }
    return false;
}

