#pragma once
#include "Collider.h"
class CColliderSphere2D :
    public CCollider
{
	friend class CGameObject;
	friend class CObject;
protected:
	CColliderSphere2D();
	CColliderSphere2D(const CColliderSphere2D& _Ref);
	CColliderSphere2D(CColliderSphere2D&& _Ref) noexcept;

public:
	~CColliderSphere2D();

protected:
	FSphere2DInfo m_Sphere2DInfo;

public:
	const FSphere2DInfo& GetInfo() const { return m_Sphere2DInfo; }

public:
	void SetDebugDraw(bool _DebugDraw);




	void SetRadius(float _R) { m_Sphere2DInfo.Radius = _R; }

public:
	virtual bool Init() override;
	virtual void Update(double _DeltaTime) override;
	virtual void PostUpdate(double _DeltaTime) override;

protected:
	virtual CColliderSphere2D* Clone() const;

public:
	virtual bool Collision(FVector3& _HitPoint, std::shared_ptr<CCollider> _Dest) override;

};

