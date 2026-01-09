#pragma once
#include "Engine/Object/GameObject.h"


class CMeshComponent;
class CCameraComponent;
class CAnimation2DComponent;
class CChargeEffect;
class CRotationMovementComponent;
class CColliderBox2D;
class CColliderSphere2D;
class CColliderLine2D;
class CColliderPolygon2D;

class CDamageController;

class CPlayer :
    public CGameObject
{
    friend class CWorld;
    friend class CObject;

protected:
    CPlayer();
    CPlayer(const CPlayer& _Ref);
    CPlayer(CPlayer&& _Ref) noexcept;

public:
    virtual ~CPlayer();

private:
    std::weak_ptr<CMeshComponent>                       m_MeshComponent;
    std::weak_ptr<CMeshComponent>                       m_Rot;
    std::weak_ptr<CCameraComponent>                     m_CameraComponent;
    std::weak_ptr<CAnimation2DComponent>                m_Animation2DComponent;
    std::weak_ptr<CRotationMovementComponent>	        m_Movement;
    std::weak_ptr<CColliderBox2D>                       m_ColliderBox;
    std::weak_ptr<CColliderSphere2D>                    m_ColliderSphere;
    std::weak_ptr<CColliderLine2D>                      m_ColliderLine;
    std::weak_ptr<CColliderPolygon2D>                   m_ColliderPolygon;


    std::shared_ptr<CDamageController>      m_DC;

    float	                                m_FireTime;
    bool	                                m_Attack = false;

    std::weak_ptr<CChargeEffect>	        m_ChargeEffect;	

    double m_MaxMP;
    double m_MP;

public:
    std::weak_ptr<CDamageController> GetDC() { return m_DC; }

public:
    virtual bool Init() override;
    virtual void Update(double _DeltaTime) override;

private:
    void RegainMP();

private:
    void TestNotify();
    void AttackNotify();
    void AttackFinish();

    void Shild();

private:
    void MoveUp();
    void MoveDown();
    void RotateLeft();
    void RotateRight();

    void Idle();

    void Skill1Press();
    void Skill1Hold();
    void Skill1Release();
};

