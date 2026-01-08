#pragma once
#include "Engine\Object\GameObject.h"

class CChargeEffect :
    public CGameObject
{
    friend class CWorld;
    friend class CObject;

protected:
    CChargeEffect();
    CChargeEffect(const CChargeEffect& _Ref);
    CChargeEffect(CChargeEffect&& _Ref) noexcept;

public:
    virtual ~CChargeEffect();

private:
    std::weak_ptr<class CMeshComponent>           m_MeshComponent;
    std::weak_ptr<class CAnimation2DComponent>    m_Animation2DComponent;

public:
    virtual bool Init() override;
    virtual void Update(double _DeltaTime) override;
};

