#pragma once
#include "Object.h"

enum class EComponentType : unsigned char
{
    Object,
    Scene,
    End
};

class CWorld;
class CGameObject;

class CComponent :
    public CObject
{
    friend class CGameObject;
protected:
    CComponent();
    CComponent(const CComponent& _Ref);
    CComponent(CComponent&& _Ref) noexcept;

public:
    virtual ~CComponent();

protected:
    std::weak_ptr<CWorld>           m_World;
    std::weak_ptr<CGameObject>      m_Owner;
    std::weak_ptr<CComponent>       m_Self;
    std::string                     m_Name;
    bool                            m_Enable;
    bool                            m_Alive;
    EComponentType                  m_ComponentType;

public:
    const std::string GetName() const { return m_Name; }
    bool GetEnable() const { return m_Enable; }
    bool GetAlive() const { return m_Alive; }
    std::weak_ptr<CWorld> GetWorld() { return m_World; }
    std::weak_ptr<CGameObject> GetOwner() { return m_Owner; }
    EComponentType GetComponentType() const { return m_ComponentType; }

    void SetEnable(bool _Enable) { m_Enable = _Enable; }
    virtual void SetSelf(std::weak_ptr<CComponent> _Self) { m_Self = _Self; }
    virtual void SetWorld(std::weak_ptr<CWorld> _World) { m_World = _World; }
    virtual void SetOwner(std::weak_ptr<CGameObject> _Owner) { m_Owner = _Owner; }
    void SetName(const std::string& _Name){ m_Name = _Name; }


public:
    virtual bool Init();
    virtual void Begin();
    virtual void Update(double DeletaTime);
    virtual void PostUpdate(double _DeltaTime);
    virtual void Render();
    virtual void Destroy();

protected:
    virtual CComponent* Clone();
public:
    template <typename T>
    std::weak_ptr<T> GetSelf()	const
    {
        return std::dynamic_pointer_cast<T>(m_Self.lock());
    }
};

