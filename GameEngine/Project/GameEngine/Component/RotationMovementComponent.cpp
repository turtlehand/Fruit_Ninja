#include "pch.h"
#include "RotationMovementComponent.h"
#include "SceneComponent.h"

CRotationMovementComponent::CRotationMovementComponent() :
	CMovementComponent(),
	m_MoveDir(FVector3::Zero),
	m_Speed(0.f),
	m_Velocity(FVector3::Zero),
	m_RotAxis(FVector3::Zero),
	m_RotSpeed(1.f)
{
}

CRotationMovementComponent::CRotationMovementComponent(const CRotationMovementComponent& _Ref) :
	CMovementComponent(),
	m_MoveDir(_Ref.m_MoveDir),
	m_Speed(_Ref.m_Speed),
	m_Velocity(_Ref.m_Velocity),
	m_RotAxis(_Ref.m_RotAxis),
	m_RotSpeed(_Ref.m_RotSpeed)
{
}

CRotationMovementComponent::CRotationMovementComponent(CRotationMovementComponent&& _Ref) noexcept:
	CMovementComponent(),
	m_MoveDir(std::move(_Ref.m_MoveDir)),
	m_Speed(std::move(_Ref.m_Speed)),
	m_Velocity(std::move(_Ref.m_Velocity)),
	m_RotAxis(std::move(_Ref.m_RotAxis)),
	m_RotSpeed(std::move(_Ref.m_RotSpeed))
{
}

CRotationMovementComponent::~CRotationMovementComponent()
{
}

bool CRotationMovementComponent::Init()
{
	return CMovementComponent::Init();
}

void CRotationMovementComponent::Update(double _DeltaTime)
{
	CMovementComponent::Update(_DeltaTime);
}

void CRotationMovementComponent::PostUpdate(double _DeltaTime)
{
	if (m_UpdateComponent.expired())
	{
		m_Velocity = FVector3::Zero;
		return;
	}

	Move(_DeltaTime);
	Rotation(_DeltaTime);
	

}

void CRotationMovementComponent::Destroy()
{
	CMovementComponent::Destroy();
}

CRotationMovementComponent* CRotationMovementComponent::Clone()	const
{
	return new CRotationMovementComponent(*this);
}

void CRotationMovementComponent::Move(double _DeltaTime)
{
	if (m_MoveDir.IsZero())
	{
		m_Velocity = FVector3::Zero;
		return;
	}

	// 방향은 정규화시킨다.
	m_MoveDir.Normalize();

	// 방향 * 속력 * DT를 곱하여 속력을 구한다.
	m_Velocity = m_MoveDir * m_Speed;

	auto	UpdateComponent = m_UpdateComponent.lock();

	UpdateComponent->AddRelativePos(m_Velocity * _DeltaTime);
	m_MoveDir = FVector3::Zero;

}

void CRotationMovementComponent::Rotation(double _DeltaTime)
{
	if (m_RotAxis.IsZero())
	{
		m_RotAxis = FVector3::Zero;
		return;
	}

	auto	UpdateComponent = m_UpdateComponent.lock();

	// 회전
	m_RotAxis.Normalize();
	UpdateComponent->AddRelativeRotation(m_RotAxis * m_RotSpeed * _DeltaTime);
	m_RotAxis = FVector3::Zero;

}
