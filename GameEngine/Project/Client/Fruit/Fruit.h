#pragma once
#include "Engine\Object\GameObject.h"

class CMeshComponent;
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
    std::weak_ptr<CMeshComponent>                       m_MeshComponent;
    std::weak_ptr<CColliderPolygon2D>                   m_PolygonCollider;

public:
    virtual bool Init() override;
    virtual void Begin() override;
    virtual void Update(double _DeltaTime) override;

public:
    void CreatePentagon();

private:
    void SplitCollider();

    void CollisionSlash();

    void CollisionBegin(const std::vector<FVector3>& _HitPoint, class CCollider* _Dest);
};

