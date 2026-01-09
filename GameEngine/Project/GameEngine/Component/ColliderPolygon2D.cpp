#include "pch.h"
#include "ColliderPolygon2D.h"

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
#include "ColliderBox2D.h"
#include "ColliderSphere2D.h"
#include "ColliderLine2D.h"

CColliderPolygon2D::CColliderPolygon2D() :
    CCollider(EColliderType::Polygon2D),
    m_Polygon2DInfo()
{
}

CColliderPolygon2D::CColliderPolygon2D(const CColliderPolygon2D& _Ref) :
    CCollider(_Ref),
    m_Polygon2DInfo(_Ref.m_Polygon2DInfo)
{
}

CColliderPolygon2D::CColliderPolygon2D(CColliderPolygon2D&& _Ref) noexcept :
    CCollider(std::move(_Ref)),
    m_Polygon2DInfo(std::move(_Ref.m_Polygon2DInfo))
{ 
}

CColliderPolygon2D::~CColliderPolygon2D()
{
}

/// <summary>
/// m_Polygon2DInfo의 Points에 점을 추가하고
/// 메쉬의 정점과 인덱스를 변경한다.
/// </summary>
/// <param name="_Point"></param>
void CColliderPolygon2D::AddPoint(const FVector3& _Point)
{
    // 더 이상 추가 불가능
    if (m_MaxPoint == m_Polygon2DInfo.Points.size())
    {
        assert(false);
        return;
    }

    m_Polygon2DInfo.Points.push_back(_Point);

    // 속이 빈 다각형
    std::vector<FVector3> CenterFrame(m_MaxPoint, FVector3::Zero);
    std::vector<unsigned short> CenterFrameIdx(m_MaxPoint + 1, 0);

    for (int i = 0; i < m_Polygon2DInfo.Points.size(); ++i)
    {
        CenterFrame[i] = m_Polygon2DInfo.Points[i];
        CenterFrameIdx[i] = i;
    }

    m_PolygonMesh->ChangeVertexBuffer(CenterFrame.data(), sizeof(FVector3), CenterFrame.size());
    m_PolygonMesh->ChangeIndexBuffer(0, CenterFrameIdx.data(), sizeof(unsigned short), CenterFrameIdx.size());
}

/// <summary>
/// _Index의 정점을 _Point로 수정한다.
/// </summary>
/// <param name="_Point"></param>
void CColliderPolygon2D::ChangeVertex(int _Index, const FVector3& _Point)
{
    // 잘못된 범위
    if (!(0 <= _Index && _Index < m_MaxPoint))
    {
        assert(false);
        return;
    }

    // Info를 변경한다.
    m_Polygon2DInfo.Points[_Index] = _Point;

    // 속이 빈 다각형
    // 정점을 Info로 변경한다.
    std::vector<FVector3> CenterFrame(m_MaxPoint, FVector3::Zero);

    for (int i = 0; i < m_Polygon2DInfo.Points.size(); ++i)
    {
        CenterFrame[i] = m_Polygon2DInfo.Points[i];
    }

    m_PolygonMesh->ChangeVertexBuffer(CenterFrame.data(), sizeof(FVector3), CenterFrame.size());
}

void CColliderPolygon2D::SetDebugDraw(bool _DebugDraw)
{
    CCollider::SetDebugDraw(_DebugDraw);

    if (_DebugDraw && m_Shader.expired())
    {
        std::shared_ptr<CShaderManager> ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock();

        m_Shader = ShaderMgr->FindShader("Collider");

        m_ColliderCBuffer.reset(new CCBufferCollider);

        m_ColliderCBuffer->Init();
    }
}

bool CColliderPolygon2D::Init()
{
    CCollider::Init();

    m_Polygon2DInfo.Points.push_back(FVector3(-0.5f, 0.5f, 0.f));
    m_Polygon2DInfo.Points.push_back(FVector3(0.5f, 0.5f, 0.f));
    m_Polygon2DInfo.Points.push_back(FVector3(0.5f, -0.5f, 0.f));
    m_Polygon2DInfo.Points.push_back(FVector3(-0.5f, -0.5f, 0.f));

    m_Mesh = CreateMesh();

    SetDebugDraw(m_DebugDraw);
    return true;
}

void CColliderPolygon2D::Update(double _DeltaTime)
{
    CCollider::Update(_DeltaTime);

}

void CColliderPolygon2D::PostUpdate(double _DeltaTime)
{
    CCollider::PostUpdate(_DeltaTime);

    m_Polygon2DInfo.Center = m_WorldPos;
    m_Polygon2DInfo.WorldPoint.resize(m_Polygon2DInfo.Points.size());
    // 사각형을 구성하는 4개의 꼭지점을 구한다.
    std::vector<FVector3> Pos;

    for (int i = 0; i < m_Polygon2DInfo.Points.size(); ++i)
    {
        FMatrix matWorld;

        if (m_InheritScale)
        {
            matWorld = m_WorldMatrix;
        }
        else
        {
            matWorld = m_RotMatrix * m_TranslateMatrix;
        }
        FVector3 Point = m_Polygon2DInfo.Points[i];
        FVector4 Point4 = matWorld * FVector4(Point.x,Point.y, Point.z, 1.f);
        Point = FVector3(Point4.x, Point4.y, Point4.z);
        Pos.push_back(Point);
        m_Polygon2DInfo.WorldPoint[i] = Point;

    }

    m_Min = Pos[0];
    m_Max = Pos[0];

    // 4개의 꼭지점을 구하고 각 꼭지점의 최소 최대를 비교하여 Min,Max를 채운다.
    for (int i = 1; i < Pos.size(); ++i)
    {
        // 최소값으로 지정된 x가 점의 x보다 클 경우 교체한다.
        m_Min.x = m_Min.x > Pos[i].x ? Pos[i].x : m_Min.x;
        m_Min.y = m_Min.y > Pos[i].y ? Pos[i].y : m_Min.y;

        m_Max.x = m_Max.x < Pos[i].x ? Pos[i].x : m_Max.x;
        m_Max.y = m_Max.y < Pos[i].y ? Pos[i].y : m_Max.y;
    }

    m_RenderScale.x = m_WorldScale.x;
    m_RenderScale.y = m_WorldScale.y;
    m_RenderScale.z = 1.f;
}

CColliderPolygon2D* CColliderPolygon2D::Clone()	const
{
    return new CColliderPolygon2D(*this);
}

/// <summary>
/// m_MaxPoint 만큼 정점 있는 메쉬를 만든다.
/// </summary>
/// <returns></returns>
std::weak_ptr<class CMesh> CColliderPolygon2D::CreateMesh()
{
    // 속이 빈 사각형
    std::vector<FVector3> CenterFrame(m_MaxPoint, FVector3::Zero);
    std::vector<unsigned short> CenterFrameIdx(m_MaxPoint + 1, 0);

    for (int i = 0; i < m_Polygon2DInfo.Points.size(); ++i)
    {
        CenterFrame[i] = m_Polygon2DInfo.Points[i];
        CenterFrameIdx[i] = i;
    }

    std::shared_ptr<CMesh> Mesh;

    Mesh.reset(new CMesh);

    if (!Mesh->CreateMesh(CenterFrame.data(), sizeof(FVector3), m_MaxPoint, D3D11_USAGE_DYNAMIC, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
        CenterFrameIdx.data(), sizeof(unsigned short), m_MaxPoint + 1, DXGI_FORMAT_R16_UINT, D3D11_USAGE_DYNAMIC))
    {
        assert(false);
        return std::weak_ptr<CMesh>();
    }

    m_PolygonMesh = Mesh;

    return Mesh;
}

bool CColliderPolygon2D::Collision(std::vector<FVector3>& _HitPoint, std::shared_ptr<CCollider> _Dest)
{
    // 상대방의 충돌체 모양이 무엇이냐에 따라 충돌 알고리즘이 달라진다.
    switch (_Dest->GetColliderType())
    {
    case EColliderType::Line2D:
        return CCollision::CollisionPolygon2DToLine2D(_HitPoint, this, dynamic_cast<CColliderLine2D*>(_Dest.get()));
        break;
    }
    return false;
}