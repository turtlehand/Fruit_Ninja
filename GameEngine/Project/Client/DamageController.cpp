#include "pch.h"
#include "DamageController.h"
#include "Engine/Object/GameObject.h"

CDamageController::CDamageController(std::weak_ptr<class CGameObject> _Owner) :
	m_Owner(_Owner),
	m_HP(10),
	m_MaxHP(10)
{
}

CDamageController::~CDamageController()
{
}

void CDamageController::Damaged(float _Value)
{
	m_HP -= _Value;

	if (m_HP <= 0)
	{
		auto Owner = m_Owner.lock();
	}
}
