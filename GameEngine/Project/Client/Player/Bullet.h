#pragma once
#include "Engine/Object/GameObject.h"

class CMeshComponent;

class CBullet :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;
protected:
	CBullet();
	CBullet(const CBullet& _Ref);
	CBullet(CBullet&& _Ref)	noexcept;

public:
	virtual ~CBullet();

protected:
	std::weak_ptr<CMeshComponent>			m_MeshComponent;
	std::weak_ptr<class CColliderBox2D>		m_Collider;
	FVector3								m_Dir;
	float									m_Distance;
	float									m_Speed;

	std::string								m_CollisionTargetName;
	std::list<std::weak_ptr<CGameObject>>	m_CollisionTarget;
	float									m_CollisionRange = 0.f;

public:
	void SetCollisionName(const std::string& _Name);
	void SetDir(const FVector3& _Dir) { m_Dir = _Dir; }
	void SetDistance(float _Dis) { m_Distance = m_Distance; }
	void SetSpeed(float _Speed) { m_Speed = _Speed; }


public:
	void ComputeCollisionRange()
	{
		FVector3	Scale = GetWorldScale();

		Scale /= 2.f;

		m_CollisionRange = sqrtf(Scale.x * Scale.x + Scale.y * Scale.y);
	}

	void SetCollisionTargetName(const std::string& _Name);

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);

protected:
	virtual CBullet* Clone();

protected:
	void CalculateDistance(double _DeltaTime);
	void CalculateCollision();

	void CollisionBegin(const FVector3& _HitPoint, class CCollider* _Dest);
	void CollisionEnd(class CCollider* _Dest);

};

