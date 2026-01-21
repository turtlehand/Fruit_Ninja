#pragma once

#include "Collider.h"

struct PolygonNode
{
	FVector3 Point;             // 현재 정점

	bool IsIntersection = false;        // 교점 여부
	int PairPointIndex = -1;         // 짝꿍 정점
};

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
	int m_MaxPoint = DYNAMIC_MESH_MAX_POINT;
	std::shared_ptr<class CMesh> m_PolygonMesh;
	FPolygon2DInfo		m_Polygon2DInfo;

public:
	const FPolygon2DInfo& GetInfo()	const
	{
		return m_Polygon2DInfo;
	}

	int GetPointCount(int _Path) const { return m_Polygon2DInfo.LocalPoints[_Path].size(); }
	int GetAllPointCount() const;


	void ResizePath(int _PathSize);
	void AddPoint(const FVector3& _Point, int _Path = 0);
	void SetPoint(int _Index, const FVector3& _Point, int _Path = 0);
	void RemovePoint(int _Path = 0);
	//bool SlicePolygon2DToLine2D_LR(const class CColliderLine2D* _LineCol, std::vector<FVector3>& _LeftPoints, std::vector<FVector3>& _RightPoints);
	bool SlicePolygon2DToLine2D(const class CColliderLine2D* _LineCol, std::vector<std::vector<FVector3>>& _Points);
	bool SlicePolygon2DToLine2D(const class CColliderLine2D* _LineCol, const std::vector<FVector3>& _PolygonPoints, std::vector<std::vector<FVector3>>& _Points);

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
	void UpdateMesh();
	void AddInterSectionPoints(const class CColliderLine2D* _LineCol, const std::vector<FVector3>& _PolygonPoints,
		std::vector<PolygonNode>& _AllPoints);
	void SortIntersectionPoints(const class CColliderLine2D* _LineCol, std::vector<PolygonNode>& _AllPoints);
	
};

