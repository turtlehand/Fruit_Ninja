#pragma once


class CDamageController
{
public:
	CDamageController(std::weak_ptr<class CGameObject> _Owner);
	~CDamageController();

private:
	std::weak_ptr<class CGameObject> m_Owner;
	float m_MaxHP;
	float m_HP;

public:
	void Damaged(float _Value);
	
};

