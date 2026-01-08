#include "pch.h"
#include "GuidedM.h"

#include "Device.h"
#include "World/World.h"
#include "World/CameraManager.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"

CGuidedM::CGuidedM() :
	CBullet(),
	m_Targets(),
	m_Target()
{
	SetClassType<CGuidedM>();
}

CGuidedM::CGuidedM(const CGuidedM& _Ref) :
	CBullet(_Ref),
	m_Targets(_Ref.m_Targets),
	m_Target(_Ref.m_Target)
{
}

CGuidedM::CGuidedM(CGuidedM&& _Ref) noexcept :
	CBullet(std::move(_Ref)),
	m_Targets(std::move(_Ref.m_Targets)),
	m_Target(std::move(_Ref.m_Target))
{
}

CGuidedM::~CGuidedM()
{
}


void CGuidedM::FindTarget()
{

}

void CGuidedM::ClosedTarget()
{
	auto World = m_World.lock();

	World->FindObjectList("Monster", m_Targets);

	if (m_Targets.empty())
		return;

	std::list<std::weak_ptr<CGameObject>>::iterator iter = m_Targets.begin();
	std::list<std::weak_ptr<CGameObject>>::iterator iterEnd = m_Targets.end();

	std::list<std::weak_ptr<CGameObject>>::iterator CurIter = iter++;
	float MinDis = (GetWorldPos() - CurIter->lock()->GetWorldPos()).Length();

	for (; iter != iterEnd; ++iter)
	{
		float Dis = (GetWorldPos() - iter->lock()->GetWorldPos()).Length();

		if (Dis < MinDis)
		{
			MinDis = Dis;
			CurIter = iter;
		}
	}

	m_Target = CurIter->lock();

}

bool CGuidedM::Init()
{
	CBullet::Init();

	FindTarget();

	return true;
}

void CGuidedM::Update(double _DeltaTime)
{
	ClosedTarget();

	FVector3	Move = !m_Target.expired() ? m_Target.lock()->GetWorldPos() - GetWorldPos() : m_Dir;
	Move.Normalize();

	m_Dir = Move;

	CalculateCollision();
	CalculateDistance(_DeltaTime);
}

CGuidedM* CGuidedM::Clone()
{
	return new CGuidedM(*this);
}
