#include "pch.h"
#include "Fruit.h"

#include "World/World.h"
#include "World/WorldAssetManager.h"
#include "Asset/Texture/Texture.h"

#include "Component/MeshComponent.h"
#include "Component/DynamicMeshComponent.h"
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
    m_MeshComponent = CreateComponent<CDynamicMeshComponent>("Root");
	m_PolygonCollider = CreateComponent<CColliderPolygon2D>("Collider");

    auto MeshC = m_MeshComponent.lock();

    if (MeshC)
    {
        MeshC->SetShader("DefaultTexture2D");
        MeshC->SetMaterialBaseColor(0, FVector4(1.0f, 1.0f, 1.0f, 1.0f));
        //MeshC->AddTexture(0, "Apple", TEXT("Apple.png"));
        MeshC->SetBlendState(0, "AlphaBlend");
		MeshC->SetSimulatePhysics(true);
		MeshC->SetUseGravity(false);
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
	CGameObject::Update(_DeltaTime);

	auto Mesh = m_MeshComponent.lock();

	if (Mesh)
	{
		//Mesh->AddRelativeRotationZ(100 * _DeltaTime);
		FVector3 CurPos = Mesh->GetWorldPos();

		if (CurPos.y < -500.f)
			Destroy();
	}
}

#pragma region CreatePolygon

void CFruit::CreateRegularPolygon(int _n, float _Radius)
{
	if (_n < 3) return; // 최소 삼각형부터 가능

	auto ColliderPolygon = m_PolygonCollider.lock();
	auto DMesh = m_MeshComponent.lock();

	if (ColliderPolygon && DMesh)
	{
		float Radius = _Radius;
		// 360도(2 * PI)를 n으로 나누어 한 내각의 크기를 구함
		float AngleStep = (2.0f * 3.1415926535f) / (float)_n;

		for (int i = 0; i < _n; ++i)
		{
			// 현재 점의 각도 (12시 방향부터 시작하려면 -PI/2를 더하거나 x,y를 바꿈)
			// 보통 12시 방향 시작은 (cos(90도), sin(90도)) 형태입니다.
			float CurrentAngle = (float)i * AngleStep;

			// 시계 방향(CW)으로 생성 (x = sin, y = cos)
			// sin, cos을 이용해 반지름 0.5 범위 내의 좌표 계산
			float x = std::sin(CurrentAngle) * Radius;
			float y = std::cos(CurrentAngle) * Radius;

			FVector3 Point(x, y, 0.0f);

			ColliderPolygon->AddPoint(Point);
			DMesh->AddPoint(Point);
		}
	}
}

void CFruit::CreateApplePolygon()
{
	auto ColliderPolygon = m_PolygonCollider.lock();
	auto DMesh = m_MeshComponent.lock();

	if (ColliderPolygon && DMesh)
	{
		// 12시 상단 홈부터 시계 방향(CW)으로 정점 정의
		// 범위는 약 -0.5f ~ 0.5f 사이로 세팅 (반지름 0.5 기준)
		FVector3 ApplePoints[] = {
			FVector3(0.00f,  0.30f, 0.0f), // 1. 상단 중앙 (쏙 들어간 홈)
			FVector3(0.15f,  0.37f, 0.0f), // 2. 오른쪽 어깨 시작
			FVector3(0.4f,  0.23f, 0.0f), // 3. 오른쪽 상단 최대 곡률
			FVector3(0.47f,  0.00f, 0.0f), // 4. 오른쪽 옆구리 (가장 넓은 곳)
			FVector3(0.34f, -0.38f, 0.0f), // 5. 오른쪽 하단 곡선
			FVector3(0.24f, -0.48f, 0.0f), // 6. 오른쪽 아래 끝
			FVector3(0.15f, -0.5f, 0.0f), // 6. 오른쪽 아래 끝
			FVector3(0.00f, -0.46f, 0.0f), // 7. 하단 중앙 (살짝 들어간 부분)
			FVector3(-0.15f, -0.5f, 0.0f), // 8. 왼쪽 아래 끝
			FVector3(-0.24f, -0.48f, 0.0f), // 8. 왼쪽 아래 끝
			FVector3(-0.34f, -0.38f, 0.0f), // 9. 왼쪽 하단 곡선
			FVector3(-0.47f,  0.00f, 0.0f), // 10. 왼쪽 옆구리
			FVector3(-0.4f,  0.23f, 0.0f), // 11. 왼쪽 상단 최대 곡률
			FVector3(-0.15f,  0.37f, 0.0f)  // 12. 왼쪽 어깨 시작
		};

		for (const auto& Pt : ApplePoints)
		{
			ColliderPolygon->AddPoint(Pt);
			DMesh->AddPoint(Pt);
		}
	}
}

void CFruit::CreateBananaPolygon()
{
	auto ColliderPolygon = m_PolygonCollider.lock();
	auto DMesh = m_MeshComponent.lock();

	if (ColliderPolygon && DMesh)
	{
		// 이미지 바운딩 박스를 기준으로 한 정밀 좌표 (-0.5 ~ 0.5 범위)
		FVector3 BananaPoints[] = {
			// 1. 좌측 하단 뭉툭한 끝부분 (Bottom Tip)
			FVector3(-0.30f, -0.30f, 0.0f),
			FVector3(-0.28f, -0.22f, 0.0f),

			// 2. 안쪽 오목한 곡선 (등 부분 - 굴곡이 완만함)
			FVector3(-0.13f, -0.15f, 0.0f),
			FVector3(0.0f,  -0.08f, 0.0f),
			FVector3(0.1f,  0.05f, 0.0f),
			FVector3(0.16f,  0.15f, 0.0f),
			FVector3(0.16f,  0.32f, 0.0f),

			// 3. 우측 상단 줄기/꼭지 (Stem - 좁고 각진 부분)
			FVector3(0.22f,  0.34f, 0.0f),
			FVector3(0.24f,  0.34f, 0.0f), // 꼭지 오른쪽 위 (단면)
			FVector3(0.25f,  0.18f, 0.0f), // 꼭지 오른쪽 위 (단면)

			// 4. 바깥쪽 볼록한 곡선 (배 부분 - 두께가 두꺼운 부분)
			FVector3(0.28f,  0.15f, 0.0f),
			FVector3(0.3f, 0.025f, 0.0f),
			FVector3(0.25f, -0.10f, 0.0f),
			FVector3(0.09f, -0.26f, 0.0f),
			FVector3(0.00f, -0.3f, 0.0f),
			FVector3(-0.15f, -0.34f, 0.0f), // 다시 시작점 근처로 연결
			FVector3(-0.20f, -0.34f, 0.0f) // 다시 시작점 근처로 연결
		};

		for (const auto& Pt : BananaPoints)
		{
			ColliderPolygon->AddPoint(Pt);
			DMesh->AddPoint(Pt);
		}
	}
}

void CFruit::CreatePentagon()
{
	auto ColliderPolygon = m_PolygonCollider.lock();
	auto DMesh = m_MeshComponent.lock();

	if (ColliderPolygon)
	{
		// [순서: 12시 -> 2시 -> 5시 -> 7시 -> 10시] - 시계 방향(CW)

		// 1. 꼭대기 (12시 방향)
		ColliderPolygon->AddPoint(FVector3(0.0f, 1.0f, 0.0f));
		DMesh->AddPoint(FVector3(0.0f, 1.0f, 0.0f));

		// 2. 오른쪽 위 (2시 방향) - x가 양수
		ColliderPolygon->AddPoint(FVector3(0.9511f, 0.3090f, 0.0f));
		DMesh->AddPoint(FVector3(0.9511f, 0.3090f, 0.0f));

		// 3. 오른쪽 아래 (5시 방향)
		ColliderPolygon->AddPoint(FVector3(0.5878f, -0.8090f, 0.0f));
		DMesh->AddPoint(FVector3(0.5878f, -0.8090f, 0.0f));

		// 4. 왼쪽 아래 (7시 방향)
		ColliderPolygon->AddPoint(FVector3(-0.5878f, -0.8090f, 0.0f));
		DMesh->AddPoint(FVector3(-0.5878f, -0.8090f, 0.0f));

		// 5. 왼쪽 위 (10시 방향) - x가 음수
		ColliderPolygon->AddPoint(FVector3(-0.9511f, 0.3090f, 0.0f));
		DMesh->AddPoint(FVector3(-0.9511f, 0.3090f, 0.0f));
	}
}

void CFruit::CreateStarPolygon()
{
	auto ColliderPolygon = m_PolygonCollider.lock();
	auto DMesh = m_MeshComponent.lock();

	if (ColliderPolygon)
	{
		float OuterRadius = 0.5f;        // 바깥쪽 반지름
		float InnerRadius = 0.2f;      // 안쪽 반지름
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
			DMesh->AddPoint(FVector3(
				cosf(OuterRad) * OuterRadius,
				sinf(OuterRad) * OuterRadius ,
				0.0f
			));

			// 2. 안쪽 점 (Inner Point)
			// 바깥쪽 점보다 36도 더 '뺀' 위치에 배치합니다.
			float InnerRad = (StartAngle - i * 72.0f - 36.0f) * DegToRad;
			ColliderPolygon->AddPoint(FVector3(
				cosf(InnerRad) * InnerRadius,
				sinf(InnerRad) * InnerRadius ,
				0.0f
			));
			DMesh->AddPoint(FVector3(
				cosf(InnerRad) * InnerRadius,
				sinf(InnerRad) * InnerRadius ,
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

void CFruit::CreateTestDoubleTriangle()
{
	auto ColliderPolygon = m_PolygonCollider.lock();

	if (ColliderPolygon)
	{
		// [순서: 11시 -> 1시 -> 5시 -> 7시] - 시계 방향(CW)
		ColliderPolygon->ResizePath(2);

		// 첫번째 삼각형
		// 1. 왼쪽 위 (11시 방향)
		ColliderPolygon->AddPoint(FVector3(-0.4f, 0.5f, 0.0f), 0);

		// 2. 오른쪽 위 (1시 방향)
		ColliderPolygon->AddPoint(FVector3(0.5f, 0.5f, 0.0f), 0);

		// 3. 오른쪽 아래 (5시 방향)
		ColliderPolygon->AddPoint(FVector3(0.5f, -0.4f, 0.0f), 0);

		// 두번째 삼각형
		// 1. 오른쪽 아래(5시 방향)
		ColliderPolygon->AddPoint(FVector3(0.4f, -0.5f, 0.0f), 1);

		// 2. 왼쪽 아래 (7시 방향)
		ColliderPolygon->AddPoint(FVector3(-0.5f, -0.5f, 0.0f), 1);

		// 3. 왼쪽 위 (11시 방향)
		ColliderPolygon->AddPoint(FVector3(-0.5f, 0.4f, 0.0f), 1);
	}
}

#pragma endregion

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
	auto Mesh = m_MeshComponent.lock();

	std::weak_ptr<CFruit> SplitFruit = World->CreateGameObject<CFruit>("Split Fruit");
	auto FLM = SplitFruit.lock()->FindComponent<CDynamicMeshComponent>("Root").lock();
	auto FLP = SplitFruit.lock()->FindComponent<CColliderPolygon2D>("Collider").lock();

	//FLM->SetTexture(0,0, Mesh->GetTexture(0, 0));

	FLM->SetRelativePos(GetRelativePos());
	FLM->SetRelativeScale(GetRelativeScale());
	FLM->SetRelativeRotation(GetRelativeRot());

	FLM->SetPhysicsVelocity(Mesh->GetPhysicsVelocity());

	for (int i = 0; i < _Points.size(); ++i)
	{
		FLP->AddPoint(_Points[i]);
		FLM->AddPoint(_Points[i]);
		FLP->PostUpdate(CTimer::GetDeltaTime());
	}
	
	return SplitFruit;
}

/// <summary>
/// 정점 배열을 
/// </summary>
/// <param name="_LeftPoints"></param>
/// <param name="_RightPoints"></param>
std::weak_ptr<CGameObject> CFruit::CreateSplitObject(const std::vector<std::vector<FVector3>>& _Polygons)
{

	//FVector3 LineVec = LineCol->GetAxis(EAxis::X);
	//LineVec = FVector3(fabs(LineVec.x), fabs(LineVec.y), 0.f);
	auto World = m_World.lock();
	auto Mesh = m_MeshComponent.lock();

	std::weak_ptr<CFruit> SplitFruit = World->CreateGameObject<CFruit>("Split Fruit");
	auto FLM = SplitFruit.lock()->FindComponent<CDynamicMeshComponent>("Root").lock();
	auto FLP = SplitFruit.lock()->FindComponent<CColliderPolygon2D>("Collider").lock();

	FLM->SetTexture(0,0, Mesh->GetTexture(0, 0));

	FLM->SetRelativePos(GetRelativePos());
	FLM->SetRelativeScale(GetRelativeScale());
	FLM->SetRelativeRotation(GetRelativeRot());

	FLM->SetPhysicsVelocity(Mesh->GetPhysicsVelocity());

	for (int i = 0; i < _Polygons.size(); ++i)
	{
		const std::vector<FVector3>& Points = _Polygons[i];
		FLP->ResizePath(_Polygons.size());

		for (int j = 0; j < Points.size(); ++j)
		{
			FLP->AddPoint(Points[j],i);
			FLM->AddPoint(Points[j]);
			FLP->PostUpdate(CTimer::GetDeltaTime());
		}
	}

	return SplitFruit;
}


/*
bool CFruit::SplitPolygon(const CColliderPolygon2D* _PolygonCol, const CColliderLine2D* _LineCol, std::vector<FVector3>& _LeftPoints, std::vector<FVector3>& _RightPoints)
{
	if (_PolygonCol == nullptr || _LineCol == nullptr)
		return false;

	const FPolygon2DInfo& PolygonColInfo = _PolygonCol->GetInfo();
	const FLine2DInfo& LineColInfo = _LineCol->GetInfo();

	_LeftPoints.push_back(PolygonColInfo.WorldPoints[0]);

	bool IsLeft = true;				// 왼쪽 정점에 넣을 차례 여부, true라면 왼쪽 정점, false라면 오른쪽 정점
	int Size = PolygonColInfo.LocalPoints.size();
	int HitIdx = 0;					// 넣을 교차점 인덱스
	std::vector<FVector3> HitPoint;	//교차점

	// 폴리곤의 선과 다른 선분의 교차점을 구해서 왼쪽, 또는 오른쪽 배열에 넣는다.
	for (int i = 0; i < Size; ++i)
	{
		int NextIdx = i == Size - 1 ? 0 : i + 1;
		FLine2DInfo PolygonLineInfo = { PolygonColInfo.WorldPoints[i], PolygonColInfo.WorldPoints[NextIdx] };

		if (CCollision::CollisionLine2DToLine2D(HitPoint, LineColInfo, PolygonLineInfo))
		{
			if (IsLeft)
			{
				_LeftPoints.push_back(HitPoint[HitIdx]);
				_RightPoints.push_back(HitPoint[HitIdx]);

				if (NextIdx != 0)
					_RightPoints.push_back(PolygonColInfo.WorldPoints[NextIdx]);
				IsLeft = false;
			}
			else
			{
				_RightPoints.push_back(HitPoint[HitIdx]);
				_LeftPoints.push_back(HitPoint[HitIdx]);
				if (NextIdx != 0)
					_LeftPoints.push_back(PolygonColInfo.WorldPoints[NextIdx]);

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
				_LeftPoints.push_back(PolygonColInfo.WorldPoints[NextIdx]);
			}
			else
			{
				_RightPoints.push_back(PolygonColInfo.WorldPoints[NextIdx]);
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
*/
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
	if (_HitPoint.size() < 2 || !m_IsBegin || _HitPoint.size() % 2 == 1)
		return;

	CColliderLine2D* LineCol = dynamic_cast<CColliderLine2D*>(_Dest);
	std::shared_ptr<CColliderPolygon2D> PolygonCol = m_PolygonCollider.lock();
	auto SMesh = m_MeshComponent.lock();

	if (!LineCol)
		return;


	std::vector<std::vector<FVector3>> Polygons;
	std::vector<std::vector<FVector3>> LeftPolygon;
	std::vector<std::vector<FVector3>> RightPolygon;
	
	if (PolygonCol->SlicePolygon2DToLine2D(LineCol, Polygons))
	{
		// 선을 Local로 변환한다.
		FLine2DInfo LocalLine = LineCol->GetInfo();
		FMatrix InvWMat = SMesh->GetWorldMatrix();
		InvWMat.Inverse();

		FVector4 LocalPoint = InvWMat * FVector4(LocalLine.Start, 1.f);
		LocalLine.Start = FVector3(LocalPoint.x, LocalPoint.y, LocalPoint.z);

		LocalPoint = InvWMat * FVector4(LocalLine.End, 1.f);
		LocalLine.End = FVector3(LocalPoint.x, LocalPoint.y, LocalPoint.z);

		for (int i = 0; i < Polygons.size(); ++i)
		{
			std::vector<FVector3>& Polygon = Polygons[i];
			bool IsLeft = true;

			for (int j = 0; j < Polygon.size(); ++j)
			{
				ECCWResult::Type CCWR = CCollision::CCW2D(LocalLine.Start, LocalLine.End, Polygon[j], 0.0001f);
				if (CCWR == ECCWResult::CW)
				{
					IsLeft = false;
					break;
				}
			}

			if (IsLeft)
			{
				LeftPolygon.push_back(Polygons[i]);
			}
			else
			{
				RightPolygon.push_back(Polygons[i]);
			}

		}

  		std::weak_ptr<CGameObject> LeftFruit = CreateSplitObject(LeftPolygon);
		std::weak_ptr<CGameObject> RightFruit = CreateSplitObject(RightPolygon);

		std::weak_ptr<CDynamicMeshComponent> LFM = LeftFruit.lock()->FindComponent<CDynamicMeshComponent>("Root");
		std::weak_ptr<CDynamicMeshComponent> RFM = RightFruit.lock()->FindComponent<CDynamicMeshComponent>("Root");

		FVector3 LineVec = LineCol->GetAxis(EAxis::X);

		LFM.lock()->AddForce(LineVec * -500);
		RFM.lock()->AddForce(LineVec * 500);

		Destroy();
	}
	

	Destroy();
}
