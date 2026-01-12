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
}

void CFruit::CreatePentagon()
{
	auto	ColliderPolygon = m_PolygonCollider.lock();

	if (ColliderPolygon)
	{
		ColliderPolygon->AddPoint(FVector3(0.0f, 1.f, 0.f));
		ColliderPolygon->AddPoint(FVector3(0.9511f, 0.3090f, 0.f));
		ColliderPolygon->AddPoint(FVector3(0.5878f, -0.8090f, 0.f));
		ColliderPolygon->AddPoint(FVector3(-0.5878f, -0.8090f, 0.f));
		ColliderPolygon->AddPoint(FVector3(-0.9511f, 0.3090f, 0.f));
	}
}

void CFruit::SplitCollider()
{



}

void CFruit::CollisionSlash()
{

}

void CFruit::CollisionBegin(const std::vector<FVector3>& _HitPoint, class CCollider* _Dest)
{
	if (2 != _HitPoint.size() || !m_IsBegin)
		return;

	CColliderLine2D* LineCol = dynamic_cast<CColliderLine2D*>(_Dest);
	std::shared_ptr<CColliderPolygon2D> PolygonCol = m_PolygonCollider.lock();

	if (!LineCol)
		return;

	bool IsLeft = true;
	std::vector<FVector3> LeftPoints;
	std::vector<FVector3> RightPoints;
	std::vector<FVector3> HitPoint;

	const FLine2DInfo& LineColInfo = LineCol->GetInfo();
	const FPolygon2DInfo& PolygonColInfo = PolygonCol->GetInfo();

	LeftPoints.push_back(PolygonColInfo.WorldPoint[0]);
	int Size = PolygonColInfo.LocalPoints.size();
	for (int i = 0; i < Size; ++i)
	{
		int NextIdx = i == Size - 1 ? 0 : i + 1;
		FLine2DInfo PolygonLineInfo = { PolygonColInfo.WorldPoint[i], PolygonColInfo.WorldPoint[NextIdx] };

		if (CCollision::CollisionLine2DToLine2D(HitPoint, LineColInfo, PolygonLineInfo))
		{
			if (IsLeft)
			{
				LeftPoints.push_back(HitPoint[0]);
				RightPoints.push_back(HitPoint[0]);

				if(NextIdx != 0)
					RightPoints.push_back(PolygonColInfo.WorldPoint[NextIdx]);
				IsLeft = false;
			}
			else
			{
				RightPoints.push_back(HitPoint[1]);
				LeftPoints.push_back(HitPoint[1]);
				if (NextIdx != 0)
					LeftPoints.push_back(PolygonColInfo.WorldPoint[NextIdx]);

				IsLeft = true;
			}
		}
		else
		{
			if (NextIdx == 0)
				break;

			if (IsLeft)
			{
				LeftPoints.push_back(PolygonColInfo.WorldPoint[NextIdx]);
			}
			else
			{
				RightPoints.push_back(PolygonColInfo.WorldPoint[NextIdx]);
			}
		}
	}


	LeftPoints;
	RightPoints; 
	// ===============================
	// Local·Î º¯È¯


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

		FRM->SetRelativePos(GetRelativePos());
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
