#include "pch.h"
#include "ProjectileMovementComponent.h"
#include "SceneComponent.h"

CProjectileMovementComponent::CProjectileMovementComponent() :
	CMovementComponent(),
	m_MoveDir(FVector3::Zero),
	m_Speed(0.f),
	m_Velocity(FVector3::Zero),
	m_Range(1.f)
{
}

CProjectileMovementComponent::CProjectileMovementComponent(const CProjectileMovementComponent& _Ref) :
	CMovementComponent(),
	m_MoveDir(_Ref.m_MoveDir),
	m_Speed(_Ref.m_Speed),
	m_Velocity(_Ref.m_Velocity),
	m_Range(_Ref.m_Range)
{
}

CProjectileMovementComponent::CProjectileMovementComponent(CProjectileMovementComponent&& _Ref) noexcept :
	CMovementComponent(),
	m_MoveDir(std::move(_Ref.m_MoveDir)),
	m_Speed(std::move(_Ref.m_Speed)),
	m_Velocity(std::move(_Ref.m_Velocity)),
	m_Range(std::move(_Ref.m_Range))
{
}

CProjectileMovementComponent::~CProjectileMovementComponent()
{
}

bool CProjectileMovementComponent::Init()
{
	return CMovementComponent::Init();
}

void CProjectileMovementComponent::Update(double _DeltaTime)
{
	CMovementComponent::Update(_DeltaTime);
}

void CProjectileMovementComponent::PostUpdate(double _DeltaTime)
{
	if (m_UpdateComponent.expired())
	{
		m_Velocity = FVector3::Zero;
		return;
	}

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

	// 사정거리에 다다르면 함수를 호출시킨다.
	if (m_Range > 0.f)
	{
		m_Range -= m_Speed * (float)_DeltaTime;
		if (m_Range <= 0.f)
		{
			if (m_RangeFunction)
				m_RangeFunction();
			m_MoveDir = FVector3::Zero;
		}
	}
}

void CProjectileMovementComponent::Destroy()
{
	CMovementComponent::Destroy();
}

CProjectileMovementComponent* CProjectileMovementComponent::Clone()	const
{
	return new CProjectileMovementComponent(*this);
}