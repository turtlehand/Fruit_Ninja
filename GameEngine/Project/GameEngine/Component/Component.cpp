#include "pch.h"
#include "Component.h"

CComponent::CComponent() :
	CObject(),
	m_World(),
	m_Owner(),
	m_Name(),
	m_Enable(true),
	m_Alive(true),
	m_ComponentType(EComponentType::End)
{
}

CComponent::CComponent(const CComponent& _Ref) :
	CObject(_Ref),
	m_World(),
	m_Owner(),
	m_Name(_Ref.m_Name),
	m_Enable(_Ref.m_Enable),
	m_Alive(_Ref.m_Alive),
	m_ComponentType(_Ref.m_ComponentType)
{
}

CComponent::CComponent(CComponent&& _Ref) noexcept:
	CObject(std::move(_Ref)),
	m_World(),
	m_Owner(),
	m_Name(_Ref.m_Name),
	m_Enable(_Ref.m_Enable),
	m_Alive(_Ref.m_Alive),
	m_ComponentType(_Ref.m_ComponentType)
{
	_Ref.m_World.reset();
	_Ref.m_Owner.reset();
}

CComponent::~CComponent()
{
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Begin()
{

}


void CComponent::Update(double DeletaTime)
{

}

void CComponent::PostUpdate(double DeletaTime)
{

}

void CComponent::Render()
{

}

CComponent* CComponent::Clone()
{
	return new CComponent(*this);
}

void CComponent::Destroy()
{
	m_Alive = false;
}
