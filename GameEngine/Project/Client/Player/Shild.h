#pragma once
#include "Engine\Object\GameObject.h"
class CShild :
    public CGameObject
{
	friend class CWorld;
	friend class CObject;
protected:
	CShild();
	CShild(const CShild& _Ref);
	CShild(CShild&& _Ref)	noexcept;

public:
	virtual ~CShild();

protected:
	std::weak_ptr<class CMeshComponent>				m_MeshComponent;
	std::weak_ptr<class CRotationMovementComponent> m_Movement;

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);

protected:
	virtual CShild* Clone();

};

