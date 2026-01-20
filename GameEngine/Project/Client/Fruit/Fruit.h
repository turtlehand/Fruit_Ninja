#pragma once
#include "Engine\Object\GameObject.h"

class CMeshComponent;
class CDynamicMeshComponent;
class CColliderPolygon2D;

// 충돌한 베기의 정보
struct FSlashInfo
{
	std::vector<FVector3> HitPoint;
};

class CFruit :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CFruit();
	CFruit(const CFruit& _Ref);
	CFruit(CFruit&& _Ref) noexcept;

public:
	virtual ~CFruit();

private:
	bool                                                m_IsBegin;
	std::weak_ptr<CDynamicMeshComponent>				m_MeshComponent;
	std::weak_ptr<CColliderPolygon2D>                   m_PolygonCollider;

public:
	virtual bool Init() override;
	virtual void Begin() override;
	virtual void Update(double _DeltaTime) override;

public:
	void CreateRegularPolygon(int _n, float _Radius);
	void CreatePentagon();
	void CreateStarPolygon();
	void CreateTriangle();
	void CreateRectangle();
	void CreateTestDoubleTriangle();

	void CreateApplePolygon();
	void CreateBananaPolygon();


private:
	std::weak_ptr<CGameObject> CreateSplitObject(const std::vector<FVector3>& _Points);
	std::weak_ptr<CGameObject> CreateSplitObject(const std::vector<std::vector<FVector3>>& _Polygons);
	//bool SplitPolygon(const class CColliderPolygon2D* _PolygonCol,const class CColliderLine2D* _LineCol, std::vector<FVector3>& _LeftPoints, std::vector<FVector3>& _RightPoints);

	void CollisionBegin(const std::vector<FVector3>& _HitPoint, class CCollider* _Dest);
};

