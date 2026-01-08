#pragma once
#include "Engine\Object\GameObject.h"

class CDamageController;

class CMonster :
    public CGameObject
{
	friend class CWorld;
	friend class CObject;
protected:
	CMonster();
	CMonster(const CMonster& ref);
	CMonster(CMonster&& ref)	noexcept;

public:
	virtual ~CMonster();

private:
	std::weak_ptr<class CMeshComponent>	m_MeshComponent;
	std::weak_ptr<class CAnimation2DComponent> m_Animation2DComponent;
	std::weak_ptr<class CColliderSphere2D>	m_Collider;
	std::weak_ptr<class CColliderBox2D>	m_BoxCollider;
	std::weak_ptr<class CColliderLine2D>                      m_ColliderLine;
	std::weak_ptr<CGameObject>			m_Player;
	float								m_FireTime;

	float								m_DetectDis;

	FVector3							m_TargetDir;
	float								m_TargetAngle;
	float								m_TargetDis;

public:
	std::shared_ptr<CDamageController> m_DC;

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);

protected:
	virtual CMonster* Clone();

private:
	bool TargetInfo();
	void Shot();

	void AttackNotify();
	void AttackFinish();


};

