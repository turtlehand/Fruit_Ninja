#pragma once
#include "Engine\Object\GameObject.h"

class CFruitManager :
    public CGameObject
{
    friend class CWorld;
    friend class CObject;

protected:
    CFruitManager();
    CFruitManager(const CFruitManager& _Ref);
    CFruitManager(CFruitManager&& _Ref) noexcept;

public:
    virtual ~CFruitManager();

private:
    //std::weak_ptr<class CCameraComponent>               m_CameraC;
    std::list<class CFruit>                             m_Fruits;

public:
    virtual bool Init() override;
    virtual void Update(double _DeltaTime) override;

private:
    void SpawnFruit();

};

