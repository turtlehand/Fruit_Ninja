#pragma once

#include "Player/Bullet.h"

class CGuidedM :
	public CBullet
{
	friend class CWorld;
	friend class CObject;
protected:
	CGuidedM();
	CGuidedM(const CGuidedM& _Ref);
	CGuidedM(CGuidedM&& _Ref)	noexcept;

public:
	virtual ~CGuidedM();

private:
	std::list<std::weak_ptr<CGameObject>>	m_Targets;
	std::weak_ptr<CGameObject>				m_Target;
public:
	void FindTarget();
	void ClosedTarget();

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);

protected:
	virtual CGuidedM* Clone();
};

