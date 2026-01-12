#include "pch.h"
#include "Slash.h"

#include "World/World.h"
#include "World/WorldAssetManager.h"
#include "Asset/Texture/Texture.h"

#include "Component/MeshComponent.h"
#include "Component/ColliderPolygon2D.h"
#include "Component/ColliderLine2D.h"
#include "Component/RotationMovementComponent.h"

CSlash::CSlash()
{
}

CSlash::CSlash(const CSlash& _Ref)
{
}

CSlash::CSlash(CSlash&& _Ref) noexcept
{
}

CSlash::~CSlash()
{
}

bool CSlash::Init()
{
	CGameObject::Init();

	m_MeshComponent = CreateComponent<CMeshComponent>("Mesh");
	m_LineCollider = CreateComponent<CColliderLine2D>("Collider");
	m_MoveComponent = CreateComponent<CRotationMovementComponent>("Movement");

	auto	ColliderLine = m_LineCollider.lock();

	if (ColliderLine)
	{
		ColliderLine->SetCollisionProfile("Fruit");

		ColliderLine->SetLineDir(FVector3::Axis[EAxis::Y]);
		ColliderLine->SetLineDistance(200.f);

		ColliderLine->SetDebugDraw(true);
		ColliderLine->SetInheritScale(false);
		ColliderLine->SetEnable(true);
	}

	auto Movement = m_MoveComponent.lock();

	if (Movement)
	{
		Movement->SetUpdateComponent(m_MeshComponent);
	}

	// 플레이어에서 사용할 키를 등록한다.
	auto	World = m_World.lock();

	auto	Input = World->GetInput().lock();

	Input->AddBindFunction<CSlash>("MoveUp", 'W', EInputType::Hold, this, &CSlash::MoveUp);
	Input->AddBindFunction<CSlash>("MoveDown", 'S', EInputType::Hold, this, &CSlash::MoveDown);
	Input->AddBindFunction<CSlash>("RotateLeft", 'A', EInputType::Hold, this, &CSlash::RotateLeft);
	Input->AddBindFunction<CSlash>("RotateRight", 'D', EInputType::Hold, this, &CSlash::RotateRight);

	return true;
}

void CSlash::Update(double _DeltaTime)
{
	CGameObject::Update(_DeltaTime);
}

void CSlash::MoveUp()
{
	auto Mesh = m_MeshComponent.lock();
	auto Movement = m_MoveComponent.lock();
	Movement->AddMove(Mesh->GetAxis(EAxis::Y));
	Movement->SetSpeed(100);
}

void CSlash::MoveDown()
{
	auto Mesh = m_MeshComponent.lock();
	auto Movement = m_MoveComponent.lock();
	Movement->AddMove(-Mesh->GetAxis(EAxis::Y));
	Movement->SetSpeed(100);
}

void CSlash::RotateLeft()
{
	auto Mesh = m_MeshComponent.lock();
	auto Movement = m_MoveComponent.lock();
	Movement->SetRotAxis(Mesh->GetAxis(EAxis::Z));
	Movement->SetRotSpeed(100);
}

void CSlash::RotateRight()
{
	auto Mesh = m_MeshComponent.lock();
	auto Movement = m_MoveComponent.lock();
	Movement->SetRotAxis(-Mesh->GetAxis(EAxis::Z));
	Movement->SetRotSpeed(100);

}