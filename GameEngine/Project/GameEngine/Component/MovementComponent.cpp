#include "pch.h"
#include "MovementComponent.h"
#include "SceneComponent.h"

CMovementComponent::CMovementComponent() :
	CObjectComponent(),
	m_UpdateComponent()
{
}

CMovementComponent::CMovementComponent(const CMovementComponent& _Ref) :
	CObjectComponent(_Ref),
	m_UpdateComponent()
{
}

CMovementComponent::CMovementComponent(CMovementComponent&& _Ref) noexcept :
	CObjectComponent(std::move(_Ref)),
	m_UpdateComponent(std::move(_Ref.m_UpdateComponent))
{
}

CMovementComponent::~CMovementComponent()
{
}

bool CMovementComponent::Init()
{
	return CObjectComponent::Init();
}

void CMovementComponent::Update(double _DeltaTime)
{
	CObjectComponent::Update(_DeltaTime);
}

void CMovementComponent::PostUpdate(double _DeltaTime)
{

}

void CMovementComponent::Destroy() 
{
	CObjectComponent::Destroy();
}

CMovementComponent* CMovementComponent::Clone()	const
{
	return new CMovementComponent(*this);
}