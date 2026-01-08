#include "pch.h"
#include "Shild.h"

#include "Engine/Component/MeshComponent.h"
#include "Engine/Component/RotationMovementComponent.h"

CShild::CShild()
{
}

CShild::CShild(const CShild& _Ref)
{
}

CShild::CShild(CShild&& _Ref) noexcept
{
}

CShild::~CShild()
{
}

bool CShild::Init()
{
	CGameObject::Init();

	m_MeshComponent = CreateComponent<CMeshComponent>("Shild");
	m_Movement = CreateComponent<CRotationMovementComponent>("Movement");

	auto Mesh = m_MeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMaterialBaseColor(0, FVector4(0.0f, 0.0f, 1.0f, 1.f));
		//Mesh->AddTexture(0, "Player", TEXT("teemo.png"));
		Mesh->SetBlendState(0, "AlphaBlend");
	}








	return true;
}

void CShild::Update(double _DeltaTime)
{

}

CShild* CShild::Clone()
{
	return new CShild(*this);
}