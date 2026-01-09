#pragma once

#include "Collider.h"

class CColliderLine2D :
	public CCollider
{
	friend class CGameObject;
	friend class CObject;

protected:
	CColliderLine2D();
	CColliderLine2D(const CColliderLine2D& _Ref);
	CColliderLine2D(CColliderLine2D&& _Ref)	noexcept;

public:
	virtual ~CColliderLine2D();

protected:
	FLine2DInfo		m_Line2DInfo;
	FVector3		m_LineDir;
	float			m_Distance = 100.f;

public:
	const FLine2DInfo& GetInfo()	const
	{
		return m_Line2DInfo;
	}

public:
	void SetLineDir(const FVector3& _Dir)
	{
		m_LineDir = _Dir;
		m_LineDir.Normalize();
	}

	void SetLineDir(float _x, float _y, float _z)
	{
		m_LineDir = FVector3(_x, _y, _z);
		m_LineDir.Normalize();
	}

	void SetLineDir(float _x, float _y)
	{
		m_LineDir = FVector3(_x, _y, 0.f);
		m_LineDir.Normalize();
	}

	void SetLineDistance(float _Dist)
	{
		m_Distance = _Dist;
	}

public:
	virtual void SetDebugDraw(bool DebugDraw);

public:
	virtual bool Init() override;
	virtual void Update(double _DeltaTime) override;
	virtual void PostUpdate(double _DeltaTime) override;

protected:
	virtual CColliderLine2D* Clone()	const;

public:
	virtual bool Collision(std::vector<FVector3>& _HitPoint, std::shared_ptr<CCollider> Dest) override;
};

