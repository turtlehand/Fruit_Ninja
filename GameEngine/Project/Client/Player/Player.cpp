#include "pch.h"
#include "Player.h"
#include "Engine/Timer.h"
#include "Engine/Component/MeshComponent.h"
#include "Engine/World/World.h"
#include "Engine/World/WorldManager.h"
#include "Engine/Component/CameraComponent.h"
#include "Engine/Component/Animation2DComponent.h"
#include "Engine/Component/MovementComponent.h"
#include "Engine/Component/RotationMovementComponent.h"
#include "Engine/Component/ColliderBox2D.h"
#include "Engine/Component/ColliderSphere2D.h"
#include "Engine/Component/ColliderLine2D.h"
#include "Engine/Component/ColliderPolygon2D.h"


#include "Bullet.h"
#include "Missile.h"
#include "Device.h"
#include "GuidedM.h"
#include "DamageController.h"
#include "ChargeEffect.h"
#include "BulletEffect.h"


CPlayer::CPlayer() :
	CGameObject(),
	m_FireTime(0.5f),
	m_MaxMP(100),
	m_MP(100)
{
	SetClassType<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& _Ref) :
	CGameObject(_Ref),
	m_FireTime(_Ref.m_FireTime),
	m_MaxMP(_Ref.m_MaxMP),
	m_MP(_Ref.m_MP)
{
}

CPlayer::CPlayer(CPlayer&& _Ref) noexcept :
	CGameObject(std::move(_Ref)),
	m_FireTime(std::move(_Ref.m_FireTime)),
	m_MaxMP(_Ref.m_MaxMP),
	m_MP(_Ref.m_MP)
{
}

CPlayer::~CPlayer()
{

}

bool CPlayer::Init()
{
	CGameObject::Init();
	m_MeshComponent = CreateComponent<CMeshComponent>("Player");
	m_Rot = CreateComponent<CMeshComponent>("Rot");
	m_CameraComponent = CreateComponent<CCameraComponent>("PlayerCamera");
	m_Animation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");
	m_Movement = CreateComponent<CRotationMovementComponent>("Movement");
	m_ColliderBox = CreateComponent<CColliderBox2D>("Collider");
	m_ColliderSphere = CreateComponent<CColliderSphere2D>("Collider");
	m_ColliderLine = CreateComponent<CColliderLine2D>("LineCollider");
	m_ColliderPolygon = CreateComponent<CColliderPolygon2D>("PolygonCollider");

	m_DC.reset(new CDamageController(m_Self));

	// 애니메이션 지정
	auto	Anim = m_Animation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(m_MeshComponent);

		Anim->AddAnimation("PlayerIdle");
		Anim->AddAnimation("PlayerWalk");
		Anim->AddAnimation("PlayerAttack");
		//Anim->ChangeAnimation("PlayerWalk");
		Anim->SetPlayRate("PlayerAttack", 2.f);

		//Anim->AddNotify<CPlayer>("PlayerIdle", "TestNotify",
		//	4, this, &CPlayer::TestNotify);
		Anim->AddNotify<CPlayer>("PlayerAttack", "AttackNotify", 2, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerAttack",
			this, &CPlayer::AttackFinish);

		//Anim->SetFinishNotify<CPlayer>("PlayerIdle", this,
		//	&CPlayer::TestNotify);

		//Anim->SetSymmetry("PlayerIdle", true);
		//Anim->SetSymmetry("PlayerWalk", true);

		Anim->SetLoop("PlayerIdle", true);
		Anim->SetLoop("PlayerWalk", true);
	}

	auto Mesh = m_MeshComponent.lock();

	if (Mesh)
	{
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMaterialBaseColor(0, FVector4(1.0f,1.0f,1.0f,0.5f));
		//Mesh->AddTexture(0, "Player", TEXT("teemo.png"));
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	auto Movement = m_Movement.lock();
	
	if (Movement)
	{
		Movement->SetUpdateComponent(Mesh);
	}

	auto RotMesh = m_Rot.lock();
	if (RotMesh)
	{
		RotMesh->SetMesh("CenterRectTex");
		RotMesh->SetShader("MaterialColor2D");
		RotMesh->SetRelativeScale(0.5f, 0.5f, 0.5f);
		RotMesh->AddRelativePos(Mesh->GetAxis(EAxis::Y) * 1);
		//RotMesh->SetBlendState(0, "AlphaBlend");
	}

	auto	Collider = m_ColliderBox.lock();

	if (Collider)
	{
		Collider->SetCollisionProfile("Player");
		Collider->SetBoxSize(10.f, 10.f);
		Collider->SetDebugDraw(true);
		Collider->SetInheritScale(false);
		Collider->SetEnable(false);
	}

	auto	ColliderSphere = m_ColliderSphere.lock();

	if (ColliderSphere)
	{
		ColliderSphere->SetCollisionProfile("Player");
		ColliderSphere->SetRadius(100.f);
		ColliderSphere->SetDebugDraw(true);
		ColliderSphere->SetInheritScale(false);
		ColliderSphere->SetEnable(false);
	}

	auto	ColliderLine = m_ColliderLine.lock();

	if (ColliderLine)
	{
		ColliderLine->SetCollisionProfile("Player");
		ColliderLine->SetLineDir(0.f, 100.f, 0.f);
		ColliderLine->SetDebugDraw(true);
		ColliderLine->SetInheritScale(false);
		ColliderLine->SetEnable(false);
	}

	auto	ColliderPolygon = m_ColliderPolygon.lock();

	if (ColliderPolygon)
	{
		ColliderPolygon->SetCollisionProfile("Player");

		//ColliderPolygon->ChangeVertex(0, FVector3(0.0f, 100.f, 0.f));
		//ColliderPolygon->ChangeVertex(1, FVector3(95.11f, 30.90f, 0.f));
		//ColliderPolygon->ChangeVertex(2, FVector3(58.78f, -80.90f, 0.f));
		//ColliderPolygon->ChangeVertex(3, FVector3(-58.78f, -80.90f, 0.f));
		//ColliderPolygon->AddPoint(FVector3(-95.11f, 30.90f, 0.f));

		ColliderPolygon->ChangeVertex(0, FVector3(-50.0f, 50.f, 0.f));
		ColliderPolygon->ChangeVertex(1, FVector3(50.f, 50.f, 0.f));
		ColliderPolygon->ChangeVertex(2, FVector3(-50.f, -50.f, 0.f));
		ColliderPolygon->ChangeVertex(3, FVector3(50.f, -50.f, 0.f));
		ColliderPolygon->AddPoint(FVector3(-50.f, 30.90f, 0.f));

		ColliderPolygon->SetDebugDraw(true);
		ColliderPolygon->SetInheritScale(false);
		ColliderPolygon->SetEnable(true);

		ColliderPolygon->SetCollisionFunction(this, &CPlayer::CollisionBegin);
	}

	auto Camera = m_CameraComponent.lock();

	if (Camera)
	{
		const FResolution& RS = CDevice::GetInst()->GetResolution();
		//Camera->SetRelativePos(0.f, 0.f, -5.f);
		Camera->SetProjection(ECameraProjectionType::Orthographic, 90.f,
			(float)RS.Width, (float)RS.Height, 1000.f);

		Camera->SetInheritRot(false);
		Camera->SetRelativePos(0.f, 0.f, -5.f);
	}


	// 플레이어에서 사용할 키를 등록한다.
	auto	World = m_World.lock();

	auto	Input = World->GetInput().lock();

	Input->AddBindFunction<CPlayer>("MoveUp", 'W', EInputType::Hold, this, &CPlayer::MoveUp);
	Input->AddBindFunction<CPlayer>("MoveDown", 'S', EInputType::Hold, this, &CPlayer::MoveDown);
	Input->AddBindFunction<CPlayer>("MoveUpRelease", 'W', EInputType::Release, this, &CPlayer::Idle);
	Input->AddBindFunction<CPlayer>("MoveDownRelease", 'S', EInputType::Release, this, &CPlayer::Idle);

	Input->AddBindFunction<CPlayer>("RotateLeft", 'A', EInputType::Hold, this, &CPlayer::RotateLeft);
	Input->AddBindFunction<CPlayer>("RotateRight", 'D', EInputType::Hold, this, &CPlayer::RotateRight);

	Input->AddBindFunction<CPlayer>("Skill1Press", VK_LBUTTON, EInputType::Press, this, &CPlayer::Skill1Press);
	Input->AddBindFunction<CPlayer>("Skill1Hold", VK_LBUTTON, EInputType::Hold, this, &CPlayer::Skill1Hold);
	Input->AddBindFunction<CPlayer>("Skill1Release", VK_LBUTTON, EInputType::Release, this, &CPlayer::Skill1Release);


	return true;
}

void CPlayer::Update(double _DeltaTime)
{
	CGameObject::Update(_DeltaTime);

	RegainMP();


	auto Mesh = m_MeshComponent.lock();
	//if (Mesh)
	//{
	//	Mesh->AddRelativeRotationZ(30.0 *_DeltaTime);
	//}

	auto	Anim = m_Animation2DComponent.lock();

	bool	Move = false;

	m_FireTime -= (float)_DeltaTime;

	if (m_FireTime > 0)
		return;

	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
	{
		Anim->ChangeAnimation("PlayerAttack");
		m_Attack = true;
	}

	if (GetAsyncKeyState('1') & 0x8000)
	{
		std::shared_ptr<CWorld>	World = m_World.lock();

		if (World)
		{
			std::weak_ptr<CMissile>	Missile = World->CreateGameObject<CMissile>("Missile");

			std::shared_ptr<CMissile>	MissileObj = Missile.lock();

			if (MissileObj)
			{
				MissileObj->SetRelativePos(GetWorldPos() + GetAxis(EAxis::Y) * 100.f);
				MissileObj->SetDir(Mesh->GetAxis(EAxis::Y));

				MissileObj->SetCollisionTargetName("Monster");
				MissileObj->ComputeCollisionRange();

				MissileObj->SetRelativeRotation(GetWorldRot());
				MissileObj->ChangeCamera();
			}
		}
		m_FireTime = 0.5f;
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		std::shared_ptr<CWorld>	World = m_World.lock();

		if (World)
		{

			for (int i = 0; i < 3; ++i)
			{
				std::weak_ptr<CGuidedM>	Missile = World->CreateGameObject<CGuidedM>("GuidedM");

				std::shared_ptr<CGuidedM>	MissileObj = Missile.lock();

				if (MissileObj)
				{
					FVector3 Pos = GetAxis(EAxis::Y) * 100.f + GetAxis(EAxis::X) * (50.f - 50.f * i);
					MissileObj->SetRelativePos(GetWorldPos() + Pos);
					MissileObj->SetCollisionTargetName("Monster");
					MissileObj->ComputeCollisionRange();
					//MissileObj->SetRelativeRotation(GetWorldRot());
					//MissileObj->ChangeCamera();
				}
			}

		}
		m_FireTime = 0.5f;
	}

}

void CPlayer::RegainMP()
{
	if (m_MaxMP <= m_MP)
		return ;

	m_MP = m_MP + CTimer::GetDeltaTime() * 10 < m_MaxMP ? m_MP + CTimer::GetDeltaTime() : m_MaxMP;
}

void CPlayer::TestNotify()
{
	OutputDebugString(TEXT("Test Notify\n"));
}

void CPlayer::AttackNotify()
{
	auto World = CWorldManager::GetInst()->GetWorld().lock();
 	auto Bullet = World->CreateGameObject<CBullet>("Bullet").lock();
	Bullet->SetCollisionName("PlayerAttack");
	Bullet->SetDir(GetAxis(EAxis::Y));

	auto BulletTransform = Bullet->GetRootComponent().lock();
	//BulletTransform->SetRelativeRotation(GetRelativeRot());

	Bullet->SetCollisionTargetName("Monster");
	Bullet->ComputeCollisionRange();

	BulletTransform->SetRelativePos(GetWorldPos());
	BulletTransform->AddRelativePos(GetAxis(EAxis::Y) * 100);
	m_FireTime = 1.0f;
}

void CPlayer::AttackFinish()
{
	m_Attack = false;

	auto	Anim = m_Animation2DComponent.lock();

	Anim->ChangeAnimation("PlayerIdle");
}

void CPlayer::Shild()
{

}

void CPlayer::MoveUp()
{
	auto Mesh = m_MeshComponent.lock();
	auto Movement = m_Movement.lock();
	auto Anim = m_Animation2DComponent.lock();
	Movement->AddMove(Mesh->GetAxis(EAxis::Y));
	Movement->SetSpeed(100);
	Anim->ChangeAnimation("PlayerWalk");

	auto PolyCol = m_ColliderPolygon.lock();
	FVector3 Point = PolyCol->GetInfo().LocalPoints[0];
	Point.y += 50 * CTimer::GetDeltaTime();
	PolyCol->ChangeVertex(0, Point);
}

void CPlayer::MoveDown()
{
	auto Mesh = m_MeshComponent.lock();
	auto Movement = m_Movement.lock();
	auto Anim = m_Animation2DComponent.lock();
	Movement->AddMove(-Mesh->GetAxis(EAxis::Y));
	Movement->SetSpeed(100);
	Anim->ChangeAnimation("PlayerWalk");

	auto PolyCol = m_ColliderPolygon.lock();
	FVector3 Point = PolyCol->GetInfo().LocalPoints[0];
	Point.y += -50 * CTimer::GetDeltaTime();
	//PolyCol->ChangeVertex(0, Point);
}

void CPlayer::RotateLeft()
{
	//auto Mesh = m_MeshComponent.lock();
	//auto Movement = m_Movement.lock();
	//Movement->SetRotAxis(Mesh->GetAxis(EAxis::Z));
	//Movement->SetRotSpeed(100);
	auto Mesh = m_MeshComponent.lock();
	auto Movement = m_Movement.lock();
	auto Anim = m_Animation2DComponent.lock();
	Movement->AddMove(-Mesh->GetAxis(EAxis::X));
	Movement->SetSpeed(100);
	Anim->ChangeAnimation("PlayerWalk");
}

void CPlayer::RotateRight()
{
	auto Mesh = m_MeshComponent.lock();
	auto Movement = m_Movement.lock();
	Movement->SetRotAxis(-Mesh->GetAxis(EAxis::Z));
	Movement->SetRotSpeed(100);

	//auto Mesh = m_MeshComponent.lock();
	//auto Movement = m_Movement.lock();
	//auto Anim = m_Animation2DComponent.lock();
	//Movement->AddMove(Mesh->GetAxis(EAxis::X));
	//Movement->SetSpeed(100);
	//Anim->ChangeAnimation("PlayerWalk");
}

void CPlayer::Idle()
{
	auto Anim = m_Animation2DComponent.lock();
	Anim->ChangeAnimation("PlayerIdle");
}

void CPlayer::Skill1Press()
{
	auto World = m_World.lock();

	m_ChargeEffect = World->CreateGameObject<CChargeEffect>("ChargeEffect");

	auto	_Effect = m_ChargeEffect.lock();

	_Effect->SetRelativePos(GetWorldPos());

	auto PolyCol = m_ColliderPolygon.lock();
	//PolyCol->AddPoint(FVector3(-100.f, 0.f, 0.f));
}

void CPlayer::Skill1Hold()
{
	if (m_ChargeEffect.expired())
		return;

	auto	_Effect = m_ChargeEffect.lock();

	_Effect->SetRelativePos(GetWorldPos());
	_Effect->SetRelativeScale(_Effect->GetRelativeScale() + FVector3(100.f, 100.f, 0.f) * CTimer::GetDeltaTime());
}

void CPlayer::Skill1Release()
{
	FVector3 Scale = m_ChargeEffect.lock()->GetWorldScale();

	m_ChargeEffect.lock()->Destroy();
	m_ChargeEffect.reset();

	auto World = CWorldManager::GetInst()->GetWorld().lock();
	auto Bullet = World->CreateGameObject<CBullet>("Bullet").lock();
	Bullet->SetDir(GetAxis(EAxis::Y));

	auto BulletTransform = Bullet->GetRootComponent().lock();
	//BulletTransform->SetRelativeRotation(GetRelativeRot());

	Bullet->SetCollisionTargetName("Monster");
	Bullet->ComputeCollisionRange();

	BulletTransform->SetRelativeScale(Scale);
	BulletTransform->SetRelativePos(GetWorldPos());
	BulletTransform->AddRelativePos(GetAxis(EAxis::Y) * 100);
	m_FireTime = 1.0f;
}

void CPlayer::CollisionBegin(const std::vector<FVector3>& _HitPoint, CCollider* _Dest)
{

	for (int i = 0; i < _HitPoint.size(); ++i)
	{
		auto World = CWorldManager::GetInst()->GetWorld().lock();
		auto Effect = World->CreateGameObject<CBulletEffect>("Effect").lock();

		Effect->SetRelativePos(_HitPoint[i]);
	}
}

void CPlayer::CollisionEnd(CCollider* _Dest)
{
}
