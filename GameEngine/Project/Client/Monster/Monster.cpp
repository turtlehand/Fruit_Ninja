#include "pch.h"
#include "Monster.h"
#include "Engine/Component/MeshComponent.h"
#include "Engine/World/WorldManager.h"
#include "Engine/World/World.h"
#include "Player/Bullet.h"
#include "DamageController.h"
#include "Engine/Component/Animation2DComponent.h"
#include "Engine/Component/ColliderSphere2D.h"
#include "Engine/Component/ColliderLine2D.h"
#include "Engine/Component/ColliderBox2D.h"

CMonster::CMonster() :
	CGameObject(),
	m_MeshComponent(),
	m_Player(),
	m_FireTime(1.5f),
	m_DetectDis(300.f),
	m_TargetDir(),
	m_TargetAngle(),
	m_TargetDis()
{
}

CMonster::CMonster(const CMonster& ref) :
	CGameObject(ref),
	m_MeshComponent(),
	m_Player(),
	m_FireTime(ref.m_FireTime),
	m_DetectDis(ref.m_DetectDis),
	m_TargetDir(),
	m_TargetAngle(),
	m_TargetDis()
{
}

CMonster::CMonster(CMonster&& ref) noexcept :
	CGameObject(std::move(ref)),
	m_MeshComponent(std::move(ref.m_MeshComponent)),
	m_Player(),
	m_FireTime(ref.m_FireTime),
	m_DetectDis(ref.m_DetectDis),
	m_TargetDir(),
	m_TargetAngle(),
	m_TargetDis()
{
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	CGameObject::Init();
	m_MeshComponent = CreateComponent<CMeshComponent>("Monster");
	m_Animation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");

	m_DC.reset(new CDamageController(m_Self));

	auto Anim = m_Animation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(m_MeshComponent);

		Anim->AddAnimation("MonsterIdle");
		Anim->AddAnimation("MonsterAttack");

		Anim->AddNotify<CMonster>("MonsterAttack", "AttackNotify", 8, this, &CMonster::AttackNotify);
		Anim->SetFinishNotify<CMonster>("MonsterAttack", this, &CMonster::AttackFinish);

		Anim->SetLoop("MonsterIdle", true);


	}

	auto Mesh = m_MeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetRelativeScale(200.f, 200.f);
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetShader("DefaultTexture2D");
		//Mesh->SetMaterialBaseColor(0, FVector4(1.f,0.f,0.f,0.5f));
		Mesh->SetBlendState(0, "AlphaBlend");

	}

	m_Collider = CreateComponent<CColliderSphere2D>("Body");
	auto	Collider = m_Collider.lock();

	if (Collider)
	{
		Collider->SetCollisionProfile("Monster");
		//Collider->SetBoxSize(100.f, 100.f);
		Collider->SetRadius(sqrtf(100.f * 100.f + 100.f * 100.f) * 0.5f);
		Collider->SetPivot(0.f, 0.f);
		Collider->SetDebugDraw(true);
		Collider->SetInheritScale(false);
		Collider->SetEnable(false);
	}

	m_BoxCollider = CreateComponent<CColliderBox2D>("Body");
	auto	BoxCollider = m_BoxCollider.lock();

	if (BoxCollider)
	{
		BoxCollider->SetCollisionProfile("Monster");
		//Collider->SetBoxSize(100.f, 100.f);
		BoxCollider->SetBoxSize(100.f, 100.f);
		BoxCollider->SetPivot(0.f, 0.f);
		BoxCollider->SetDebugDraw(true);
		BoxCollider->SetInheritScale(false);
		BoxCollider->SetEnable(false);
	}

	m_ColliderLine = CreateComponent<CColliderLine2D>("ColliderLine");
	auto	ColliderLine = m_ColliderLine.lock();

	if (ColliderLine)
	{
		ColliderLine->SetCollisionProfile("Monster");
		ColliderLine->SetLineDir(0.f, 1000.f, 0.f);
		ColliderLine->SetLineDistance(1000.f);
		ColliderLine->SetDebugDraw(true);
		ColliderLine->SetInheritScale(false);
		//ColliderLine->SetEnable(false);
	}

	auto World = m_World.lock();

	m_Player = World->FindObject<CGameObject>("Player");

	return true;
}

void CMonster::Update(double _DeltaTime)
{
	CGameObject::Update(_DeltaTime);
	m_FireTime -=(float) _DeltaTime;

	if (TargetInfo())
	{
		if (m_FireTime <= 0.f)
		{
			auto	Anim = m_Animation2DComponent.lock();

			Anim->ChangeAnimation("MonsterAttack");

			m_FireTime = 1.5f;
		}
	}
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

/// <summary>
/// 타겟과의 정보
/// </summary>
/// <returns></returns>
bool CMonster::TargetInfo()
{
	auto Player = m_Player.lock();

	// 플레이어 쪽 방향 벡터
	m_TargetDir = Player->GetWorldPos() - GetWorldPos();
	m_TargetDir.Normalize();

	// 플레이어의 쪽 방향 각도 위쪽을 기준으로
	m_TargetAngle = FVector3::GetAngle2D(FVector3::Axis[EAxis::X], m_TargetDir);

	// 타겟과의 거리
	m_TargetDis = (Player->GetWorldPos() - GetWorldPos()).Length();

	// 사정거리
	if (m_TargetDis < m_DetectDis)
		return true;
	return false;
}

void CMonster::Shot()
{
	//SetRelativeRotationZ(m_TargetAngle);

	auto World = CWorldManager::GetInst()->GetWorld().lock();
	auto Bullet = World->CreateGameObject<CBullet>("Bullet").lock();
	Bullet->SetCollisionName("MonsterAttack");
	Bullet->SetDir(m_TargetDir);
	Bullet->SetCollisionTargetName("Player");
	Bullet->ComputeCollisionRange();

	auto BulletTransform = Bullet->GetRootComponent().lock();
	//BulletTransform->SetRelativeRotationZ(m_TargetAngle);
	BulletTransform->SetRelativePos(GetWorldPos());
	BulletTransform->AddRelativePos(m_TargetDir);

}


void CMonster::AttackNotify()
{
	if (TargetInfo())
	{
		//SetRelativeRotationZ(m_TargetAngle);
		Shot();
	}
}

void CMonster::AttackFinish()
{
	auto	Anim = m_Animation2DComponent.lock();

	if (Anim)
	{
		Anim->ChangeAnimation("MonsterIdle");
	}
}