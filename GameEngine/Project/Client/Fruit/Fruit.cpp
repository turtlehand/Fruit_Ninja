#include "pch.h"
#include "Fruit.h"

#include "World/WorldAssetManager.h"
#include "Asset/Texture/Texture.h"

#include "Component/MeshComponent.h"
#include "Component/ColliderPolygon2D.h"

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

		ColliderPolygon->AddPoint(FVector3(0.0f, 1.f, 0.f));
		ColliderPolygon->AddPoint(FVector3(0.9511f, 0.3090f, 0.f));
		ColliderPolygon->AddPoint(FVector3(0.5878f, -0.8090f, 0.f));
		ColliderPolygon->AddPoint(FVector3(-0.5878f, -0.8090f, 0.f));
		ColliderPolygon->AddPoint(FVector3(-0.9511f, 0.3090f, 0.f));

		ColliderPolygon->SetDebugDraw(true);
		ColliderPolygon->SetInheritScale(true);
		ColliderPolygon->SetEnable(true);
	}

    return true;
}

void CFruit::Update(double _DeltaTime)
{
}

void CFruit::SplitCollider()
{



}

void CFruit::CollisionSlash()
{

}

void CFruit::CollisionBegin(const std::vector<FVector3>& _HitPoint, class CCollider* _Dest)
{
	int a = 0;
}
