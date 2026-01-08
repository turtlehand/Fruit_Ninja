#include "pch.h"
#include "Missile.h"

#include "Device.h"
#include "World/World.h"
#include "World/CameraManager.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"

CMissile::CMissile()
{
}

CMissile::CMissile(const CMissile& _Ref) :
	CBullet(_Ref)
{
}

CMissile::CMissile(CMissile&& _Ref) noexcept :
	CBullet(std::move(_Ref))
{
}

CMissile::~CMissile()
{
}

void CMissile::ChangeCamera()
{
	auto	World = m_World.lock();

	if (World)
	{
		auto	CameraManager = World->GetCameraManager().lock();

		if (CameraManager)
			CameraManager->ChangeMainCamera(m_CameraComponent);
	}
}

bool CMissile::Init()
{
	CBullet::Init();

	m_CameraComponent = CreateComponent<CCameraComponent>("MissileCamera");
	auto	Camera = m_CameraComponent.lock();

	if (Camera)
	{
		const FResolution& RS = CDevice::GetInst()->GetResolution();
		//Camera->SetRelativePos(0.f, 0.f, -5.f);
		Camera->SetProjection(ECameraProjectionType::Orthographic, 90.f,
			(float)RS.Width, (float)RS.Height, 1000.f);

		Camera->SetInheritRot(false);
		Camera->SetRelativePos(0.f, 0.f, -10.f);

	}

	return true;
}

void CMissile::Update(double _DeltaTime)
{
	CalculateCollision();
	CalculateDistance(_DeltaTime);
}

CMissile* CMissile::Clone()
{
	return new CMissile(*this);
}
