#pragma once
#include "ObjectComponent.h"
#include "Timer.h"

class CMovementComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CMovementComponent();
	CMovementComponent(const CMovementComponent& _Ref);
	CMovementComponent(CMovementComponent&& _Ref)	noexcept;

public:
	virtual ~CMovementComponent();

protected:
	std::weak_ptr<class CSceneComponent>	m_UpdateComponent;

public:
	void SetUpdateComponent(const std::weak_ptr<class CSceneComponent>& _Component) { m_UpdateComponent = _Component; }


public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);
	virtual void PostUpdate(double _DeltaTime);
	virtual void Destroy();

protected:
	virtual CMovementComponent* Clone()	const;
};

