#include "pch.h"
#include "ChargeEffect.h"
#include "Engine/Component/MeshComponent.h"
#include "Engine/Component/Animation2DComponent.h"

CChargeEffect::CChargeEffect()
{
}

CChargeEffect::CChargeEffect(const CChargeEffect& _Ref)
{
}

CChargeEffect::CChargeEffect(CChargeEffect&& _Ref) noexcept
{
}

CChargeEffect::~CChargeEffect()
{
}

bool CChargeEffect::Init()
{
	CGameObject::Init();
	m_MeshComponent = CreateComponent<CMeshComponent>("Player");
	m_Animation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");

	auto Anim = m_Animation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(m_MeshComponent);

		Anim->AddAnimation("Charge");

		Anim->SetLoop("Charge", true);
	}

	auto Mesh = m_MeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMaterialBaseColor(0, FVector4(1.0f, 1.0f, 1.0f, 0.5f));
		//Mesh->AddTexture(0, "Player", TEXT("teemo.png"));
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetRelativeScale(100.f, 100.f);
	}

	return true;
}

void CChargeEffect::Update(double _DeltaTime)
{
	CGameObject::Update(_DeltaTime);
}
