#pragma once
#include "Component/ObjectComponent.h"

class CStateComponent :
    public CObjectComponent
{
private:
    CStateComponent();
    CStateComponent(const CStateComponent& _Ref);
    CStateComponent(CStateComponent&& _Ref);
public:
    ~CStateComponent();
protected:
    int m_HP = 10;
    int m_HPMax = 10;
public:
    bool AddHP(int _Damage)
    {
        m_HP += _Damage;
        if (m_HP >= m_HPMax)
            m_HP = m_HPMax;
        else if (m_HP <= 0)
        {
            m_HP = 0;
            return false;
        }
        return true;
    }
};

