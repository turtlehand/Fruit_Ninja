#pragma once
#include "Component.h"

class CWorld;
class CObjectComponent :
    public CComponent
{
    friend class CGameObject;
protected:
    CObjectComponent();
    CObjectComponent(const CObjectComponent& _Ref);
    CObjectComponent(CObjectComponent&& _Ref) noexcept;

public:
    ~CObjectComponent();

public:
    virtual bool Init() override;
    virtual void Begin() override;
    virtual void Update(double DeltaTime) override;
    virtual void PostUpdate(double _DeltaTime) override;
    virtual void Destroy() override;
protected:
    virtual CObjectComponent* Clone() const = 0;
};

