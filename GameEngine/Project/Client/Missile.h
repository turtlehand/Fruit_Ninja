#pragma once

#include "Player/Bullet.h"

class CMissile :
	public CBullet
{
	friend class CWorld;
	friend class CObject;
protected:
	CMissile();
	CMissile(const CMissile& _Ref);
	CMissile(CMissile&& _Ref)	noexcept;

public:
	virtual ~CMissile();

protected:
	std::weak_ptr<class CCameraComponent>	m_CameraComponent;

public:
	void ChangeCamera();

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);

protected:
	virtual CMissile* Clone();
};

