#pragma once
#include "Engine\Object\GameObject.h"

class CMeshComponent;
class CColliderLine2D;
class CRotationMovementComponent;

class CSlash :
    public CGameObject
{
    friend class CWorld;
    friend class CObject;

protected:
    CSlash();
    CSlash(const CSlash& _Ref);
    CSlash(CSlash&& _Ref) noexcept;

public:
    virtual ~CSlash();

private:
    std::weak_ptr<CMeshComponent>                       m_MeshComponent;
    std::weak_ptr<CColliderLine2D>                   m_LineCollider;
    std::weak_ptr<CRotationMovementComponent>       m_MoveComponent;

public:
    virtual bool Init() override;
    virtual void Update(double _DeltaTime) override;

private:
    void MoveUp();
    void MoveDown();
    void RotateLeft();
    void RotateRight();
};