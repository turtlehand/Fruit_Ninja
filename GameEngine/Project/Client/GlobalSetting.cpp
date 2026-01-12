#include "pch.h"
#include "GlobalSetting.h"
#include "CollisionInfoManager.h"

CGlobalSetting::CGlobalSetting()
{

}

CGlobalSetting::~CGlobalSetting()
{

}

bool CGlobalSetting::Init()
{
	CCollisionInfoManager::GetInst()->CreateChannel("Fruit");
	CCollisionInfoManager::GetInst()->CreateChannel("Slash");

	CCollisionInfoManager::GetInst()->CreateProfile("Fruit", "Fruit", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Slash", "Slash", true);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("Fruit", "Fruit", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("Fruit", "Slash", ECollisionInteraction::Collision);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("Slash", "Slash", ECollisionInteraction::Ignore);

	return true;
}