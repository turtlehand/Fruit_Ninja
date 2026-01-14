#include "pch.h"
#include "Fruit.h"

#include "World/World.h"
#include "World/WorldAssetManager.h"
#include "Asset/Texture/Texture.h"

#include "Component/MeshComponent.h"
#include "Component/ColliderPolygon2D.h"
#include "Component/ColliderLine2D.h"

#include "Component/Collision.h"
#include "Engine/Timer.h"

CFruit::CFruit()
{
}

CFruit::CFruit(const CFruit& _Ref)
{
}

CFruit::CFruit(CFruit&& _Ref) noexcept
{
}

CFruit::~CFruit()
{
}

bool CFruit::Init()
{
    m_MeshComponent = CreateComponent<CMeshComponent>("Root");
	m_PolygonCollider = CreateComponent<CColliderPolygon2D>("Collider");

    auto MeshC = m_MeshComponent.lock();

    if (MeshC)
    {
        MeshC->SetMesh("CenterRectTex");
        MeshC->SetShader("DefaultTexture2D");
        MeshC->SetMaterialBaseColor(0, FVector4(1.0f, 1.0f, 1.0f, 1.0f));
        //MeshC->AddTexture(0, "Apple", TEXT("Apple.png"));
        MeshC->SetBlendState(0, "AlphaBlend");
        //MeshC->SetRelativeScale(100.f, 100.f);
    }

	auto	ColliderPolygon = m_PolygonCollider.lock();

	if (ColliderPolygon)
	{
		ColliderPolygon->SetCollisionProfile("Fruit");
		ColliderPolygon->SetDebugDraw(true);
		ColliderPolygon->SetInheritScale(true);
		ColliderPolygon->SetEnable(true);

		//ColliderPolygon->SetRelativeRotationY(180);
	}

	ColliderPolygon->SetCollisionBeginFunction<CFruit>(this, &CFruit::CollisionBegin);


    return true;
}

void CFruit::Begin()
{
	m_IsBegin = true;
}

void CFruit::Update(double _DeltaTime)
{
	auto Mesh = m_MeshComponent.lock();

	if (Mesh)
	{
		//Mesh->AddRelativeRotationZ(100 * _DeltaTime);
	}
}

void CFruit::CreatePentagon()
{
	auto ColliderPolygon = m_PolygonCollider.lock();

	if (ColliderPolygon)
	{
		// [순서: 12시 -> 2시 -> 5시 -> 7시 -> 10시] - 시계 방향(CW)

		// 1. 꼭대기 (12시 방향)
		ColliderPolygon->AddPoint(FVector3(0.0f, 1.0f, 0.0f));

		// 2. 오른쪽 위 (2시 방향) - x가 양수
		ColliderPolygon->AddPoint(FVector3(0.9511f, 0.3090f, 0.0f));

		// 3. 오른쪽 아래 (5시 방향)
		ColliderPolygon->AddPoint(FVector3(0.5878f, -0.8090f, 0.0f));

		// 4. 왼쪽 아래 (7시 방향)
		ColliderPolygon->AddPoint(FVector3(-0.5878f, -0.8090f, 0.0f));

		// 5. 왼쪽 위 (10시 방향) - x가 음수
		ColliderPolygon->AddPoint(FVector3(-0.9511f, 0.3090f, 0.0f));
	}
}

void CFruit::CreateStar()
{
	auto ColliderPolygon = m_PolygonCollider.lock();

	if (ColliderPolygon)
	{
		float OuterRadius = 1.0f;        // 바깥쪽 반지름
		float InnerRadius = 0.382f;      // 안쪽 반지름
		float StartAngle = 90.0f;        // 12시 방향 시작

		// PI 상수를 미리 정의하면 편리합니다.
		const float PI = 3.141592f;
		const float DegToRad = PI / 180.0f;

		for (int i = 0; i < 5; ++i)
		{
			// 1. 바깥쪽 점 (Outer Point)
			// i * 72.0f를 '빼서' 시계 방향으로 회전합니다.
			float OuterRad = (StartAngle - i * 72.0f) * DegToRad;
			ColliderPolygon->AddPoint(FVector3(
				cosf(OuterRad) * OuterRadius,
				sinf(OuterRad) * OuterRadius,
				0.0f
			));

			// 2. 안쪽 점 (Inner Point)
			// 바깥쪽 점보다 36도 더 '뺀' 위치에 배치합니다.
			float InnerRad = (StartAngle - i * 72.0f - 36.0f) * DegToRad;
			ColliderPolygon->AddPoint(FVector3(
				cosf(InnerRad) * InnerRadius,
				sinf(InnerRad) * InnerRadius,
				0.0f
			));
		}
	}
}

void CFruit::CreateTriangle()
{
	auto ColliderPolygon = m_PolygonCollider.lock();

	if (ColliderPolygon)
	{
		// [순서: 12시 -> 4시 -> 8시] - 시계 방향(CW)

		// 1. 꼭대기 (12시 방향)
		ColliderPolygon->AddPoint(FVector3(0.0f, 1.0f, 0.0f));

		// 2. 오른쪽 아래 (약 4시 방향)
		// x = cos(-30도) * 1 = 0.866, y = sin(-30도) * 1 = -0.5
		ColliderPolygon->AddPoint(FVector3(0.866f, -0.5f, 0.0f));

		// 3. 왼쪽 아래 (약 8시 방향)
		// x = -0.866, y = -0.5
		ColliderPolygon->AddPoint(FVector3(-0.866f, -0.5f, 0.0f));
	}
}

void CFruit::CreateRectangle()
{
	auto ColliderPolygon = m_PolygonCollider.lock();

	if (ColliderPolygon)
	{
		// [순서: 11시 -> 1시 -> 5시 -> 7시] - 시계 방향(CW)

		// 1. 왼쪽 위 (11시 방향)
		ColliderPolygon->AddPoint(FVector3(-0.5f, 0.5f, 0.0f));

		// 2. 오른쪽 위 (1시 방향)
		ColliderPolygon->AddPoint(FVector3(0.5f, 0.5f, 0.0f));

		// 3. 오른쪽 아래 (5시 방향)
		ColliderPolygon->AddPoint(FVector3(0.5f, -0.5f, 0.0f));

		// 4. 왼쪽 아래 (7시 방향)
		ColliderPolygon->AddPoint(FVector3(-0.5f, -0.5f, 0.0f));
	}
}

/// <summary>
/// 정점 배열을 
/// </summary>
/// <param name="_LeftPoints"></param>
/// <param name="_RightPoints"></param>
std::weak_ptr<CGameObject> CFruit::CreateSplitObject(const std::vector<FVector3>& _Points)
{

	//FVector3 LineVec = LineCol->GetAxis(EAxis::X);
	//LineVec = FVector3(fabs(LineVec.x), fabs(LineVec.y), 0.f);
	auto World = m_World.lock();

	std::weak_ptr<CFruit> FruitLeft = World->CreateGameObject<CFruit>("FruitLeft");
	auto FLM = FruitLeft.lock()->FindComponent<CMeshComponent>("Root").lock();
	auto FLP = FruitLeft.lock()->FindComponent<CColliderPolygon2D>("Collider").lock();

	FLM->SetRelativePos(GetRelativePos());
	FLM->SetRelativeScale(GetRelativeScale());
	FLM->SetRelativeRotation(GetRelativeRot());

	for (int i = 0; i < _Points.size(); ++i)
	{
		FLP->AddPoint(_Points[i]);
		FLP->PostUpdate(CTimer::GetDeltaTime());
	}
	
	return FruitLeft;
}

bool CFruit::SplitPolygon(const CColliderPolygon2D* _PolygonCol, const CColliderLine2D* _LineCol, std::vector<FVector3>& _LeftPoints, std::vector<FVector3>& _RightPoints)
{
	if (_PolygonCol == nullptr || _LineCol == nullptr)
		return false;

	const FPolygon2DInfo& PolygonColInfo = _PolygonCol->GetInfo();
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
	FMatrix InvWMat = _PolygonCol->GetWorldMatrix();
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

/*
void CFruit::CollisionBegin(const std::vector<FVector3>& _HitPoint, class CCollider* _Dest)
{
	if (_HitPoint.size() < 2 || !m_IsBegin)
		return;

	CColliderLine2D* LineCol = dynamic_cast<CColliderLine2D*>(_Dest);
	std::shared_ptr<CColliderPolygon2D> PolygonCol = m_PolygonCollider.lock();

	if (!LineCol)
		return;

	bool IsLeft = true;
	std::vector<FVector3> LeftPoints;
	std::vector<FVector3> RightPoints;

	const FLine2DInfo& LineColInfo = LineCol->GetInfo();
	const FPolygon2DInfo& PolygonColInfo = PolygonCol->GetInfo();
	const std::vector<FTriangle2DInfo>& TriangleInfo = PolygonColInfo.WorldTriangle;

	std::vector<FTriangle2DInfo> NewTriangleInfo;
	int Size = TriangleInfo.size();

	int HitPointIdx = 0;
	for (int i = 0; i < Size; ++i)
	{
		std::vector<FVector3> AllPoints;
		std::vector<FVector3> HitPoint;

		for (int j = 0; j < 3; ++j)
		{
			FLine2DInfo TriLine;
			int Next = j + 1 == 3 ? 0 : j + 1;

			TriLine.Start = TriangleInfo[i].Point[j];
			TriLine.End = TriangleInfo[i].Point[Next];

			AllPoints.push_back(TriangleInfo[i].Point[j]);

			if (CCollision::CollisionLine2DToLine2D(HitPoint, TriLine, LineColInfo))
			{
				AllPoints.push_back(HitPoint[HitPointIdx++]);
			}

		}
		HitPointIdx = 0;

		if (HitPoint.size() == 2)
		{
			for (int j = 0; j < 5; ++j)
			{
				if (CCollision::CCW2D(LineColInfo.Start, LineColInfo.End, AllPoints[j]) == ECCWResult::CW)
				{
					bool Equal = false;

					// 거의 같은 점이 있다면 추가하지 않는다.
					for (int k = 0; k < LeftPoints.size(); ++k)
					{
						if (MaybeEqual(LeftPoints[k].x, AllPoints[j].x) && MaybeEqual(LeftPoints[k].y, AllPoints[j].y) && MaybeEqual(LeftPoints[k].z, AllPoints[j].z))
						{
							Equal = true;
						}
					}
					if (!Equal)
						LeftPoints.push_back(AllPoints[j]);

				}
				else if (CCollision::CCW2D(LineColInfo.Start, LineColInfo.End, AllPoints[j]) == ECCWResult::CCW)
				{
					bool Equal = false;

					// 거의 같은 점이 있다면 추가하지 않는다.
					for (int k = 0; k < RightPoints.size(); ++k)
					{
						if (MaybeEqual(RightPoints[k].x, AllPoints[j].x) && MaybeEqual(RightPoints[k].y, AllPoints[j].y) && MaybeEqual(RightPoints[k].z, AllPoints[j].z))
						{
							Equal = true;
						}
					}
					if (!Equal)
						RightPoints.push_back(AllPoints[j]);
				}
				else
				{
					bool Equal = false;

					// 거의 같은 점이 있다면 추가하지 않는다.
					for (int k = 0; k < LeftPoints.size(); ++k)
					{
						if (MaybeEqual(LeftPoints[k].x, AllPoints[j].x) && MaybeEqual(LeftPoints[k].y, AllPoints[j].y) && MaybeEqual(LeftPoints[k].z, AllPoints[j].z))
						{
							Equal = true;
						}
					}
					if (!Equal)
						LeftPoints.push_back(AllPoints[j]);

					Equal = false;

					// 거의 같은 점이 있다면 추가하지 않는다.
					for (int k = 0; k < RightPoints.size(); ++k)
					{
						if (MaybeEqual(RightPoints[k].x, AllPoints[j].x) && MaybeEqual(RightPoints[k].y, AllPoints[j].y) && MaybeEqual(RightPoints[k].z, AllPoints[j].z))
						{
							Equal = true;
						}
					}
					if (!Equal)
						RightPoints.push_back(AllPoints[j]);
				}
			}
		}
		// 교차하지 않음
		else
		{
			// 시계방향은 왼쪽
			if (CCollision::CCW2D(LineColInfo.Start, LineColInfo.End, AllPoints[0]) == ECCWResult::CW)
			{
				for (int j = 0; j < 3; ++j)
				{
					bool Equal = false;

					// 거의 같은 점이 있다면 추가하지 않는다.
					for (int k = 0; k < LeftPoints.size(); ++k)
					{
						if (MaybeEqual(LeftPoints[k].x, AllPoints[j].x) && MaybeEqual(LeftPoints[k].y, AllPoints[j].y) && MaybeEqual(LeftPoints[k].y, AllPoints[j].y))
						{
							Equal = true;
						}
					}
					if (Equal)
						continue;
					LeftPoints.push_back(AllPoints[i]);
				}
			}
			else
			{
				for (int j = 0; j < 3; ++j)
				{
					bool Equal = false;

					// 거의 같은 점이 있다면 추가하지 않는다.
					for (int k = 0; k < RightPoints.size(); ++k)
					{
						if (MaybeEqual(RightPoints[k].x, AllPoints[j].x) && MaybeEqual(RightPoints[k].y, AllPoints[j].y) && MaybeEqual(RightPoints[k].y, AllPoints[j].y))
						{
							Equal = true;
						}
					}
					if (Equal)
						continue;
					RightPoints.push_back(AllPoints[i]);
				}
			}
		}
	}

	LeftPoints;
	RightPoints;
	// ===============================
	// Local로 변환


	FMatrix InvWMat = PolygonCol->GetWorldMatrix();
	InvWMat.Inverse();

	auto World = m_World.lock();
	{
		std::weak_ptr<CFruit> FruitLeft = World->CreateGameObject<CFruit>("FruitLeft");
		auto FLM = FruitLeft.lock()->FindComponent<CMeshComponent>("Root").lock();
		auto FLP = FruitLeft.lock()->FindComponent<CColliderPolygon2D>("Collider").lock();

		FLM->SetRelativePos(GetRelativePos());
		FLM->SetRelativeScale(GetRelativeScale());
		FLM->SetRelativeRotation(GetRelativeRot());

		for (int i = 0; i < LeftPoints.size(); ++i)
		{
			FVector4 LocalPoints = InvWMat * FVector4(LeftPoints[i], 1.f);
			FLP->AddPoint(FVector3(LocalPoints.x, LocalPoints.y, LocalPoints.z));
			FLP->PostUpdate(CTimer::GetDeltaTime());

		}
	}

	{
		std::weak_ptr<CFruit> FruitRight = World->CreateGameObject<CFruit>("FruitRight");
		auto FRM = FruitRight.lock()->FindComponent<CMeshComponent>("Root").lock();
		auto FRP = FruitRight.lock()->FindComponent<CColliderPolygon2D>("Collider").lock();

		FRM->SetRelativePos(GetRelativePos() + FVector3(100.f,100.f,0.f));
		FRM->SetRelativeScale(GetRelativeScale());
		FRM->SetRelativeRotation(GetRelativeRot());

		for (int i = 0; i < RightPoints.size(); ++i)
		{
			FVector4 LocalPoints = InvWMat * FVector4(RightPoints[i], 1.f);
			FRP->AddPoint(FVector3(LocalPoints.x, LocalPoints.y, LocalPoints.z));
			FRP->PostUpdate(CTimer::GetDeltaTime());
		}
	}
	Destroy();

}
*/


void CFruit::CollisionBegin(const std::vector<FVector3>& _HitPoint, class CCollider* _Dest)
{
	if (_HitPoint.size() < 2 || !m_IsBegin)
		return;

	CColliderLine2D* LineCol = dynamic_cast<CColliderLine2D*>(_Dest);
	std::shared_ptr<CColliderPolygon2D> PolygonCol = m_PolygonCollider.lock();

	if (!LineCol)
		return;

	std::vector<FVector3> LeftPoints;
	std::vector<FVector3> RightPoints;

	if (PolygonCol->SlicePolygon2DToLine2D(LineCol, LeftPoints, RightPoints))
	{
		FVector3 LineVec = LineCol->GetAxis(EAxis::X);
		LineVec = FVector3(fabs(LineVec.x), fabs(LineVec.y), 0.f);

		CreateSplitObject(LeftPoints).lock()->AddRelativePos(LineVec * 50.f);
		CreateSplitObject(RightPoints).lock()->AddRelativePos(LineVec * -50.f);
		Destroy();
	}

}
