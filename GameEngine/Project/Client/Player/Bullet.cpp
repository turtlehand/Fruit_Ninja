#include "pch.h"
#include "Player/Bullet.h"

#include "Engine/Asset/Mesh/MeshManager.h"
#include "Engine/Asset/Mesh/Mesh.h"
#include "World/WorldManager.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/ColliderBox2D.h"


#include "Player/Player.h"
#include "Monster/Monster.h"
#include "DamageController.h"

CBullet::CBullet() :
	CGameObject(),
	m_Distance(800.f),
	m_Speed(200.f)
{
	SetClassType<CBullet>();
}

CBullet::CBullet(const CBullet& _Ref) :
	CGameObject(_Ref),
	m_Distance(_Ref.m_Distance),
	m_Speed(_Ref.m_Speed)
{
}

CBullet::CBullet(CBullet&& _Ref) noexcept :
	CGameObject(std::move(_Ref)),
	m_Distance(_Ref.m_Distance),
	m_Speed(_Ref.m_Speed)
{
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	CGameObject::Init();

	m_MeshComponent = CreateComponent<CMeshComponent>("Mesh");

	auto Mesh = m_MeshComponent.lock();


	if (Mesh)
	{
		Mesh->SetRelativeScale(50.f, 50.f, 1.f);

		Mesh->SetMesh("CenterRectColor");
		Mesh->SetShader("MaterialColor2D");
		Mesh->SetMaterialBaseColor(0, FVector4::Green);
	}

	m_Collider = CreateComponent<CColliderBox2D>("Body");
	auto	Collider = m_Collider.lock();

	if (Collider)
	{
		Collider->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
		Collider->SetCollisionEndFunction<CBullet>(this, &CBullet::CollisionEnd);

		Collider->SetCollisionProfile("PlayerAttack");
		Collider->SetBoxSize(100.f, 100.f);
		Collider->SetDebugDraw(true);
		Collider->SetInheritScale(false);
	}

	return true;
}

void CBullet::Update(double _DeltaTime)
{
	//CalculateCollision();
	CalculateDistance(_DeltaTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::SetCollisionName(const std::string& _Name)
{
	auto Body = m_Collider.lock();

	if (Body)
	{
		Body->SetCollisionProfile(_Name);
	}
}

void CBullet::SetCollisionTargetName(const std::string& _Name)
{
	m_CollisionTargetName = _Name;
	auto World = m_World.lock();

	World->FindObjectList(m_CollisionTargetName, m_CollisionTarget);
}

void CBullet::CalculateDistance(double _DeltaTime)
{
	m_Dir.Normalize();
	m_Dir = m_Dir * _DeltaTime * m_Speed;

	m_Distance -= m_Dir.Length();

	AddRelativePos(m_Dir);

	//SetRelativeRotationZ(FVector3::GetAngle2D(FVector3::Axis[EAxis::X], m_Dir));

	if (m_Distance <= 0.f)
		Destroy();
}

void CBullet::CalculateCollision()
{
	ComputeCollisionRange();

	auto World = m_World.lock();

	World->FindObjectList(m_CollisionTargetName, m_CollisionTarget);

	std::list<std::weak_ptr<CGameObject>>::iterator iter = m_CollisionTarget.begin();
	std::list<std::weak_ptr<CGameObject>>::iterator iterEnd = m_CollisionTarget.end();

	for (; iter != iterEnd; ++iter)
	{
		auto GO = iter->lock();

		FVector3	Scale = GO->GetWorldScale();

		Scale /= 2.f;

		float Range = sqrtf(Scale.x * Scale.x + Scale.y * Scale.y);

		float Length = (GO->GetWorldPos() - GetWorldPos()).Length();

		if (Length - Range - m_CollisionRange <= 0)
		{
			if (m_CollisionTargetName == "Player")
			{
				auto Player = std::dynamic_pointer_cast<CPlayer>(GO);
				Player->GetDC().lock()->Damaged(1);
			}
			else
			{
				auto Monster = std::dynamic_pointer_cast<CMonster>(GO);
				Monster->m_DC->Damaged(1);
			}

			Destroy();
		}
	}
}

void CBullet::CollisionBegin(const FVector3& _HitPoint, CCollider* _Dest)
{
	Destroy();
}

void CBullet::CollisionEnd(CCollider* _Dest)
{
}
