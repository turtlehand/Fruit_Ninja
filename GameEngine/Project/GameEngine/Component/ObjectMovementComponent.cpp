#include "pch.h"
#include "ObjectMovementComponent.h"
#include "SceneComponent.h"

CObjectMovementComponent::CObjectMovementComponent() :
	CMovementComponent(),
	m_MoveDir(FVector3::Zero),
	m_Speed(0.f),
	m_Velocity(FVector3::Zero)
{
}

CObjectMovementComponent::CObjectMovementComponent(const CObjectMovementComponent& _Ref) :
	CMovementComponent(_Ref),
	m_MoveDir(_Ref.m_MoveDir),
	m_Speed(_Ref.m_Speed),
	m_Velocity(_Ref.m_Velocity)
{
}

CObjectMovementComponent::CObjectMovementComponent(CObjectMovementComponent&& _Ref) noexcept :
	CMovementComponent(std::move(_Ref)),
	m_MoveDir(std::move(_Ref.m_MoveDir)),
	m_Speed(std::move(_Ref.m_Speed)),
	m_Velocity(std::move(_Ref.m_Velocity))
{
}

CObjectMovementComponent::~CObjectMovementComponent()
{
}

bool CObjectMovementComponent::Init()
{
	return CMovementComponent::Init();
}

void CObjectMovementComponent::Update(double _DeltaTime)
{
	CMovementComponent::Update(_DeltaTime);
}

void CObjectMovementComponent::PostUpdate(double _DeltaTime)
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

	m_MoveDir = FVector3::Zero;
}

void CObjectMovementComponent::Destroy()
{
	CMovementComponent::Destroy();
}

CObjectMovementComponent* CObjectMovementComponent::Clone()	const
{
	return new CObjectMovementComponent(*this);
}