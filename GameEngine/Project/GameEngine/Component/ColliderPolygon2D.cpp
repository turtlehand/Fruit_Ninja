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
#include "DynamicMeshComponent.h"

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
    if (m_MaxPoint <= m_Polygon2DInfo.LocalPoints.size())
    {
        assert(false);
        return;
    }

    m_Polygon2DInfo.LocalPoints.push_back(_Point);


    // 정점, 인덱스 버퍼 변경하기
    // 속이 빈 다각형
    UpdateMesh();

}

/// <summary>
/// _Index의 정점을 _Point로 수정한다.
/// </summary>
/// <param name="_Point"></param>
void CColliderPolygon2D::SetPoint(int _Index, const FVector3& _Point)
{
    // 잘못된 범위
    if (!(0 <= _Index && _Index < m_MaxPoint))
    {
        assert(false);
        return;
    }

    // Info를 변경한다.
    m_Polygon2DInfo.LocalPoints[_Index] = _Point;

    // 정점, 인덱스 버퍼 변경하기
    // 속이 빈 다각형
    UpdateMesh();
}

/// <summary>
/// 마지막 점을 삭제한다.
/// </summary>
void CColliderPolygon2D::RemovePoint()
{
    // 잘못된 범위
    if (m_Polygon2DInfo.LocalPoints.empty())
    {
        assert(false);
        return;
    }

    m_Polygon2DInfo.LocalPoints.pop_back();

    // 정점, 인덱스 버퍼 변경하기
    // 속이 빈 다각형
    UpdateMesh();
}

bool CColliderPolygon2D::SlicePolygon2DToLine2D_LR(const CColliderLine2D* _LineCol, std::vector<FVector3>& _LeftPoints, std::vector<FVector3>& _RightPoints)
{
    if (_LineCol == nullptr)
        return false;

    const FPolygon2DInfo& PolygonColInfo = GetInfo();
    const FLine2DInfo& LineColInfo = _LineCol->GetInfo();

    _LeftPoints.push_back(PolygonColInfo.WorldPoint[0]);

    bool IsLeft = true;				// 왼쪽 정점에 넣을 차례 여부, true라면 왼쪽 정점, false라면 오른쪽 정점
    int Size = PolygonColInfo.LocalPoints.size();
    int HitIdx = 0;					// 넣을 교차점 인덱스
    std::vector<FVector3> HitPoint;	//교차점

    // 폴리곤의 선과 다른 선분의 교차점을 구해서 왼쪽, 또는 오른쪽 배열에 넣는다.
    for (int i = 0; i < Size; ++i)
    {
        int NextIdx = i == Size - 1 ? 0 : i + 1;
        FLine2DInfo PolygonLineInfo = { PolygonColInfo.WorldPoint[i], PolygonColInfo.WorldPoint[NextIdx] };

        if (CCollision::CollisionLine2DToLine2D(HitPoint, LineColInfo, PolygonLineInfo))
        {
            if (IsLeft)
            {
                _LeftPoints.push_back(HitPoint[HitIdx]);
                _RightPoints.push_back(HitPoint[HitIdx]);

                if (NextIdx != 0)
                    _RightPoints.push_back(PolygonColInfo.WorldPoint[NextIdx]);
                IsLeft = false;
            }
            else
            {
                _RightPoints.push_back(HitPoint[HitIdx]);
                _LeftPoints.push_back(HitPoint[HitIdx]);
                if (NextIdx != 0)
                    _LeftPoints.push_back(PolygonColInfo.WorldPoint[NextIdx]);

                IsLeft = true;
            }
            ++HitIdx;
        }
        else
        {
            if (NextIdx == 0)
                break;

            if (IsLeft)
            {
                _LeftPoints.push_back(PolygonColInfo.WorldPoint[NextIdx]);
            }
            else
            {
                _RightPoints.push_back(PolygonColInfo.WorldPoint[NextIdx]);
            }
        }
    }

    if (HitPoint.size() < 2)
        return false;

    // ===============================
    // Local로 변환
    FMatrix InvWMat = GetWorldMatrix();
    InvWMat.Inverse();

    for (int i = 0; i < _LeftPoints.size(); ++i)
    {
        FVector4 LocalPoint = InvWMat * FVector4(_LeftPoints[i], 1.f);
        _LeftPoints[i] = FVector3(LocalPoint.x, LocalPoint.y, LocalPoint.z);
    }

    for (int i = 0; i < _RightPoints.size(); ++i)
    {
        FVector4 LocalPoint = InvWMat * FVector4(_RightPoints[i], 1.f);
        _RightPoints[i] = FVector3(LocalPoint.x, LocalPoint.y, LocalPoint.z);
    }

    return true;
}

bool CColliderPolygon2D::SlicePolygon2DToLine2D(const CColliderLine2D* _LineCol, std::vector<std::vector<FVector3>>& _Points)
{
    if (_LineCol == nullptr)
        return false;

    const FPolygon2DInfo& PolygonColInfo = GetInfo();
    const FLine2DInfo& LineColInfo = _LineCol->GetInfo();
    const std::vector<FVector3>& WorldPoints = PolygonColInfo.WorldPoint;
    int Size = (int)WorldPoints.size();

    if (Size < 3) return false;

    // 1. 선의 방향에 상관없이 "0번 점이 있는 쪽"을 항상 Inside(Main)로 판정
    // 이렇게 하면 선을 어느 방향으로 긋든 Main과 New가 바뀌지 않습니다.
    float ReferenceSide = (LineColInfo.End.x - LineColInfo.Start.x) * (WorldPoints[0].y - LineColInfo.Start.y) -
        (LineColInfo.End.y - LineColInfo.Start.y) * (WorldPoints[0].x - LineColInfo.Start.x);

    auto IsInside = [&](const FVector3& P) {
        float Side = (LineColInfo.End.x - LineColInfo.Start.x) * (P.y - LineColInfo.Start.y) -
            (LineColInfo.End.y - LineColInfo.Start.y) * (P.x - LineColInfo.Start.x);
        if (ReferenceSide == 0) return Side <= 0;
        return (ReferenceSide * Side) >= 0; // 0번 점과 부호가 같으면 Inside
        };

    // 2. 루프를 돌기 전, "안전하게 Inside에 있는 점"을 시작점으로 찾음 (Rotation)
    // 0번 점이 Outside라면, Inside인 점을 찾아서 그 인덱스부터 시작합니다.
    int StartIdx = -1;
    for (int i = 0; i < Size; ++i) {
        if (IsInside(WorldPoints[i])) {
            StartIdx = i;
            break;
        }
    }

    // 모든 점이 한쪽에만 있다면 자를 필요가 없음
    if (StartIdx == -1) return false;

    // 3. 단일 패스 슬라이싱 로직
    std::vector<FVector3> MainPoly;
    std::vector<FVector3> NewPoly;
    bool inMain = true;

    // Size만큼 돌면서 모든 변을 검사 (StartIdx부터 한 바퀴)
    for (int i = 0; i < Size; ++i)
    {
        int CurIdx = (StartIdx + i) % Size;
        int NextIdx = (StartIdx + i + 1) % Size;

        const FVector3& CurP = WorldPoints[CurIdx];
        const FVector3& NextP = WorldPoints[NextIdx];

        // 현재 점 추가
        if (inMain) MainPoly.push_back(CurP);
        else NewPoly.push_back(CurP);

        // 교점 확인
        std::vector<FVector3> HitPoints;
        if (CCollision::CollisionLine2DToLine2D(HitPoints, LineColInfo, { CurP, NextP }))
        {
            FVector3 Intersect = HitPoints[0];

            if (inMain) // Inside -> Outside (진출)
            {
                MainPoly.push_back(Intersect); // Main 닫기

                NewPoly.clear();
                NewPoly.push_back(Intersect); // New 시작
                inMain = false;
            }
            else // Outside -> Inside (진입)
            {
                NewPoly.push_back(Intersect); // New 닫기
                if (NewPoly.size() >= 3) _Points.push_back(NewPoly);
                NewPoly.clear();

                MainPoly.push_back(Intersect); // Main 다시 시작
                inMain = true;
            }
        }
    }

    // 마지막 남은 MainPoly 저장
    if (MainPoly.size() >= 3)
        _Points.push_back(MainPoly);

    // ===============================
    // Local로 변환
    FMatrix InvWMat = GetWorldMatrix();
    InvWMat.Inverse();

    for (int i = 0; i < _Points.size(); ++i)
    {
        for (int j = 0; j < _Points[i].size(); ++j)
        {
            FVector4 LocalPoint = InvWMat * FVector4(_Points[i][j], 1.f);
            _Points[i][j] = FVector3(LocalPoint.x, LocalPoint.y, LocalPoint.z);
        }
    }

    return !_Points.empty();
}

/*
bool CColliderPolygon2D::SlicePolygon2DToLine2D(const CColliderLine2D* _LineCol, std::vector<std::vector<FVector3>>& _Points)
{
    if (_LineCol == nullptr)
        return false;

    const FPolygon2DInfo& PolygonColInfo = GetInfo();
    const FLine2DInfo& LineColInfo = _LineCol->GetInfo();

    // ===============================
    // Local로 변환
    FMatrix InvWMat = GetWorldMatrix();
    InvWMat.Inverse();

    for (int i = 0; i < _Points.size(); ++i)
    {
        for (int j = 0; j < _Points[i].size(); ++j)
        {
            FVector4 LocalPoint = InvWMat * FVector4(_Points[i][j], 1.f);
            _Points[i][j] = FVector3(LocalPoint.x, LocalPoint.y, LocalPoint.z);
        }
    }

    return true;
}
*/

void CColliderPolygon2D::SetDebugDraw(bool _DebugDraw)
{
    CCollider::SetDebugDraw(_DebugDraw);

    if (_DebugDraw && m_Shader.expired())
    {
        std::shared_ptr<CShaderManager> ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock();

        m_Shader = ShaderMgr->FindShader("WireFrame");

        m_ColliderCBuffer.reset(new CCBufferCollider);

        m_ColliderCBuffer->Init();
    }
}

bool CColliderPolygon2D::Init()
{
    CCollider::Init();

    //m_Polygon2DInfo.Points.push_back(FVector3(-0.5f, 0.5f, 0.f));
    //m_Polygon2DInfo.Points.push_back(FVector3(0.5f, 0.5f, 0.f));
    //m_Polygon2DInfo.Points.push_back(FVector3(0.5f, -0.5f, 0.f));
    //m_Polygon2DInfo.Points.push_back(FVector3(-0.5f, -0.5f, 0.f));

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
    m_Polygon2DInfo.WorldPoint.resize(m_Polygon2DInfo.LocalPoints.size());

    FMatrix matWorld;
    std::vector<FVector3> Pos;

    // World 매트릭스를 가져오거나 만든다.
    if (m_InheritScale)
    {
        matWorld = m_WorldMatrix;
    }
    else
    {
        matWorld = m_RotMatrix * m_TranslateMatrix;
    }

    // Local에 World Matrix를 곱하여 World 좌표를 추가한다.
    for (int i = 0; i < m_Polygon2DInfo.LocalPoints.size(); ++i)
    {
        FVector3 Point = m_Polygon2DInfo.LocalPoints[i];
        FVector4 Point4 = matWorld * FVector4(Point.x,Point.y, Point.z, 1.f);
        Point = FVector3(Point4.x, Point4.y, Point4.z);
        Pos.push_back(Point);
        m_Polygon2DInfo.WorldPoint[i] = Point;
    }

    //m_Polygon2DInfo.WorldTriangle.resize(m_Polygon2DInfo.LocalTriangle.size());

    //for (int i = 0; i < m_Polygon2DInfo.LocalTriangle.size(); ++i)
    //{
    //    for (int j = 0; j < 3; ++j)
    //    {
    //        FVector3 Point = m_Polygon2DInfo.LocalTriangle[i].Point[j];
    //        FVector4 Point4 = matWorld * FVector4(Point.x, Point.y, Point.z, 1.f);
    //        Point = FVector3(Point4.x, Point4.y, Point4.z);
    //        m_Polygon2DInfo.WorldTriangle[i].Point[j] = Point;
    //    }
    //}

    // 사각형을 구성하는 4개의 꼭지점을 구한다.

    if (!Pos.empty())
    {
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
    }


    m_RenderScale.x = m_WorldScale.x;
    m_RenderScale.y = m_WorldScale.y;
    m_RenderScale.z = 1.f;
}

CColliderPolygon2D* CColliderPolygon2D::Clone()	const
{
    return new CColliderPolygon2D(*this);
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

/// <summary>
/// MaxPoint 만큼 정점 있는 메쉬를 만든다.
/// </summary>
/// <returns></returns>
std::weak_ptr<class CMesh> CColliderPolygon2D::CreateMesh()
{
    // 속이 빈 사각형
    std::vector<FVector3> CenterFrame(m_MaxPoint, FVector3::Zero);
    std::vector<unsigned short> CenterFrameIdx((m_MaxPoint - 2) * 3, 0);

    for (int i = 0; i < m_Polygon2DInfo.LocalPoints.size(); ++i)
    {
        CenterFrame[i] = m_Polygon2DInfo.LocalPoints[i];
        CenterFrameIdx[i] = i;
    }

    std::shared_ptr<CMesh> Mesh;

    Mesh.reset(new CMesh);

    if (!Mesh->CreateMesh(CenterFrame.data(), sizeof(FVector3), m_MaxPoint, D3D11_USAGE_DYNAMIC, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        CenterFrameIdx.data(), sizeof(unsigned short), (m_MaxPoint - 2) * 3, DXGI_FORMAT_R16_UINT, D3D11_USAGE_DYNAMIC))
    {
        assert(false);
        return std::weak_ptr<CMesh>();
    }

    m_PolygonMesh = Mesh;

    return Mesh;
}

/// <summary>
/// 분할된 삼각형으로 메쉬의 정점, 인덱스를 변경한다.
/// </summary>
/// <param name="_Triangle"></param>
void CColliderPolygon2D::UpdateMesh()
{
    // 더 이상 추가 불가능
    if (m_MaxPoint <= m_Polygon2DInfo.LocalPoints.size())
    {
        assert(false);
        return;
    }

    // 삼각형 분할
    //m_Polygon2DInfo.LocalTriangle.clear();
    std::vector<FTriangle2DInfo> Triangle;
    CCollision::EarClipping(m_Polygon2DInfo.LocalPoints, Triangle);

    // 정점, 인덱스 버퍼 변경하기
    // 속이 빈 다각형
    std::vector<FVector3> CenterFrame;
    std::vector<unsigned short> CenterFrameIdx;

    for (int i = 0; i < Triangle.size(); ++i)
    {

        for (int j = 0; j < 3; ++j)
        {
            // 1. 이미 정점 리스트에 있는 점인지 확인 (중복 제거)
            int FoundIndex = -1;
            for (int v = 0; v < (int)CenterFrame.size(); ++v)
            {
                if (CenterFrame[v] == Triangle[i].Point[j]) // 근사치 비교 권장
                {
                    FoundIndex = v;
                    break;
                }
            }

            // 2. 새로운 점이면 Vertices에 추가
            if (FoundIndex == -1)
            {
                FoundIndex = (int)CenterFrame.size();
                CenterFrame.push_back(Triangle[i].Point[j]);
            }

            // 3. 인덱스 버퍼에 해당 번호 기록
            CenterFrameIdx.push_back((unsigned short)FoundIndex);

        }
    }

    CenterFrame.resize(m_MaxPoint);
    CenterFrameIdx.resize((m_MaxPoint - 2) * 3);

    m_PolygonMesh->ChangeVertexBuffer(CenterFrame.data(), sizeof(FVector3), CenterFrame.size());
    m_PolygonMesh->ChangeIndexBuffer(0, CenterFrameIdx.data(), sizeof(unsigned short), CenterFrameIdx.size());
}
