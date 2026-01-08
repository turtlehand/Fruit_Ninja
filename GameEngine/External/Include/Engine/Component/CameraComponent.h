#pragma once
#include "SceneComponent.h"

enum class ECameraProjectionType
{
    Perspective,
    Orthographic
};

class CCameraComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CObject;
protected:
    CCameraComponent();
    CCameraComponent(const CCameraComponent& _Ref);
    CCameraComponent(CCameraComponent&& _Ref) noexcept;

public:
    ~CCameraComponent();

protected:
    ECameraProjectionType   m_ProjType;
    FMatrix                 m_ViewMatrix;
    FMatrix                 m_ProjMatrix;
    float                   m_ViewAngle;
    float                   m_Width;
    float                   m_Height;
    float                   m_ViewDistance;

public:
    const FMatrix& GetViewMatrix() const { return m_ViewMatrix; }

    const FMatrix& GetProjMatrix() const { return m_ProjMatrix; }

    void SetProjection(ECameraProjectionType _Type, float _ViewAngle, float _Width, float _Height, float _ViewDistance);


public:
    virtual bool Init() override;
    virtual void Update(double _DeltaTime) override;
    virtual void PostUpdate(double _DeltaTime) override;

protected:
    virtual CCameraComponent* Clone() const;

};

