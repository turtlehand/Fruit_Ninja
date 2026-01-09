#pragma once
#include "Engine\Object\GameObject.h"

class CMeshComponent;

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
    std::weak_ptr<CMeshComponent>                       m_MeshComponent;

public:
    virtual bool Init() override;
    virtual void Update(double _DeltaTime) override;

};

