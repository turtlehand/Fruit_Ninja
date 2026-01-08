#include "pch.h"
#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{
	m_ComponentType = EComponentType::Object;
}

CObjectComponent::CObjectComponent(const CObjectComponent& _Ref) : 
	CComponent(_Ref)
{
	m_ComponentType = EComponentType::Object;
}

CObjectComponent::CObjectComponent(CObjectComponent&& _Ref) noexcept :
	CComponent(std::move(_Ref))
{
	m_ComponentType = EComponentType::Object;
}

CObjectComponent::~CObjectComponent()
{
}

bool CObjectComponent::Init()
{
	CComponent::Init();
	return true;
}

void CObjectComponent::Begin()
{
	CComponent::Begin();
}

void CObjectComponent::Update(double DeltaTime)
{
	CComponent::Update(DeltaTime);

}

void CObjectComponent::PostUpdate(double _DeltaTime)
{
	CComponent::PostUpdate(_DeltaTime);

}

void CObjectComponent::Destroy()
{
	CComponent::Destroy();
}