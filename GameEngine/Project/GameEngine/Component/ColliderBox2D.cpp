#include "pch.h"
#include "ColliderBox2D.h"

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
#include "ColliderSphere2D.h"
#include "ColliderLine2D.h"

CColliderBox2D::CColliderBox2D() :
    CCollider(EColliderType::Box2D),
    m_Box2DInfo()
{

}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& _Ref) :
    CCollider(_Ref),
    m_Box2DInfo(_Ref.m_Box2DInfo)
{
}

CColliderBox2D::CColliderBox2D(CColliderBox2D&& _Ref) noexcept :
    CCollider(std::move(_Ref)),
    m_Box2DInfo(std::move(_Ref).m_Box2DInfo)
{
}

CColliderBox2D::~CColliderBox2D()
{
}

void CColliderBox2D::SetDebugDraw(bool _Draw)
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

            m_Mesh = AssetMgr->FindMesh("CenterFrameRect");
        }
        else
        {
            std::shared_ptr<CMeshManager> MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();

            m_Mesh = MeshMgr->FindMesh("CenterFrameRect");
        }

        m_ColliderCBuffer.reset(new CCBufferCollider);

        m_ColliderCBuffer->Init();
    }
}

bool CColliderBox2D::Init()
{
    CCollider::Init();
    SetDebugDraw(m_DebugDraw);
    return true;
}

void CColliderBox2D::Update(double _DeltaTime)
{
    CCollider::Update(_DeltaTime);

}

void CColliderBox2D::PostUpdate(double _DeltaTime)
{
    CCollider::PostUpdate(_DeltaTime);

    m_Box2DInfo.Center = m_WorldPos;
    m_Box2DInfo.Axis[EAxis::X] = m_WorldAxis[EAxis::X];
    m_Box2DInfo.Axis[EAxis::Y] = m_WorldAxis[EAxis::Y];

    // 사각형을 구성하는 4개의 꼭지점을 구한다.
    FVector3 Pos[4];
    Pos[0] = m_Box2DInfo.Center - m_Box2DInfo.Axis[EAxis::X] * m_Box2DInfo.HalfSize.x -
        m_Box2DInfo.Axis[EAxis::Y] * m_Box2DInfo.HalfSize.y;
    Pos[1] = m_Box2DInfo.Center - m_Box2DInfo.Axis[EAxis::X] * m_Box2DInfo.HalfSize.x +
        m_Box2DInfo.Axis[EAxis::Y] * m_Box2DInfo.HalfSize.y;
    Pos[2] = m_Box2DInfo.Center + m_Box2DInfo.Axis[EAxis::X] * m_Box2DInfo.HalfSize.x -
        m_Box2DInfo.Axis[EAxis::Y] * m_Box2DInfo.HalfSize.y;
    Pos[3] = m_Box2DInfo.Center + m_Box2DInfo.Axis[EAxis::X] * m_Box2DInfo.HalfSize.x +
        m_Box2DInfo.Axis[EAxis::Y] * m_Box2DInfo.HalfSize.y;

    m_Min = Pos[0];
    m_Max = Pos[0];

    // 4개의 꼭지점을 구하고 각 꼭지점의 최소 최대를 비교하여 Min,Max를 채운다.
    for (int i = 1; i < 4; ++i)
    {
        // 최소값으로 지정된 x가 점의 x보다 클 경우 교체한다.
        m_Min.x = m_Min.x > Pos[i].x ? Pos[i].x : m_Min.x;
        m_Min.y = m_Min.y > Pos[i].y ? Pos[i].y : m_Min.y;

        m_Max.x = m_Max.x < Pos[i].x ? Pos[i].x : m_Max.x;
        m_Max.y = m_Max.y < Pos[i].y ? Pos[i].y : m_Max.y;
    }

    m_RenderScale.x = m_WorldScale.x * m_Box2DInfo.HalfSize.x * 2.f;
    m_RenderScale.y = m_WorldScale.y * m_Box2DInfo.HalfSize.y * 2.f;
    m_RenderScale.z = 1.f;
}

CColliderBox2D* CColliderBox2D::Clone() const
{
    return new CColliderBox2D(*this);
}


bool CColliderBox2D::Collision(std::vector<FVector3>& _HitPoint, std::shared_ptr<CCollider> _Dest)
{
    // 상대방의 충돌체 모양이 무엇이냐에 따라 충돌 알고리즘이 달라진다.
    switch (_Dest->GetColliderType())
    {
    case EColliderType::Box2D:
        // 둘다 회전이 0일 경우 AABB, 아니면 OBB 충돌을 진행한다.
        return CCollision::CollisionBox2DToBox2D(_HitPoint, this, dynamic_cast<CColliderBox2D*>(_Dest.get()));
        break;
    case EColliderType::Sphere2D:
        return CCollision::CollisionBox2DToSphere2D(_HitPoint, this, dynamic_cast<CColliderSphere2D*>(_Dest.get()));
        break;
    case EColliderType::Line2D:
        return CCollision::CollisionBox2DToLine2D(_HitPoint, this, dynamic_cast<CColliderLine2D*>(_Dest.get()));
        break;
    }
    return false;
}

