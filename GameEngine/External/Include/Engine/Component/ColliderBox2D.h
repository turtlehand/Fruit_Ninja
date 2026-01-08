#pragma once
#include "Collider.h"
class CColliderBox2D :
    public CCollider
{
	friend class CGameObject;
	friend class CObject;
protected:
	CColliderBox2D();
	CColliderBox2D(const CColliderBox2D& _Ref);
	CColliderBox2D(CColliderBox2D&& _Ref) noexcept;

public:
	~CColliderBox2D();

protected:
	FBox2DInfo m_Box2DInfo;

public:
	const FBox2DInfo& GetInfo() const { return m_Box2DInfo; }

public:
	void SetDebugDraw(bool _DebugDraw);
	void SetBoxSize(const FVector2& _Size) { m_Box2DInfo.HalfSize = _Size / 2.f; }
	void SetBoxSize(float _x, float _y) { m_Box2DInfo.HalfSize = FVector2(_x/2.f, _y/2.f); }

public:
	virtual bool Init() override;
	virtual void Update(double _DeltaTime) override;
	virtual void PostUpdate(double _DeltaTime) override;

protected:
	virtual CColliderBox2D* Clone() const;

public:
	virtual bool Collision(FVector3& _HitPoint, std::shared_ptr<CCollider> _Dest) override;

};

