#pragma once

#include "Collider.h"

class CColliderPolygon2D :
	public CCollider
{
	friend class CGameObject;
	friend class CObject;

protected:
	CColliderPolygon2D();
	CColliderPolygon2D(const CColliderPolygon2D& _Ref);
	CColliderPolygon2D(CColliderPolygon2D&& _Ref)	noexcept;

public:
	virtual ~CColliderPolygon2D();

protected:
	int m_MaxPoint = 100;
	std::shared_ptr<class CMesh> m_PolygonMesh;
	FPolygon2DInfo		m_Polygon2DInfo;

public:
	const FPolygon2DInfo& GetInfo()	const
	{
		return m_Polygon2DInfo;
	}

	int GetPointCount() const { return m_Polygon2DInfo.Points.size(); }

	void AddPoint(const FVector3& _Point);
	void ChangeVertex(int _Index, const FVector3& _Point);

public:
	virtual void SetDebugDraw(bool _DebugDraw) override;

public:
	virtual bool Init() override;
	virtual void Update(double _DeltaTime) override;
	virtual void PostUpdate(double _DeltaTime) override;

protected:
	virtual CColliderPolygon2D* Clone()	const;

public:
	virtual bool Collision(std::vector<FVector3>& _HitPoint, std::shared_ptr<CCollider> _Dest) override;

private:
	std::weak_ptr<class CMesh> CreateMesh(); 
	
};

